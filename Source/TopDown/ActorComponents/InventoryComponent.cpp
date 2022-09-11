// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/ActorComponents/InventoryComponent.h"

#include <stdexcept>

#include "Kismet/GameplayStatics.h"

#include "TopDown/Game/TopDownGameInstance.h"
#include "TopDown/Util/Logger.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
    // features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryComponent::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::SetDefaultWeapon(const FName WeaponName) {
    auto Info = CreateNewWeapon(WeaponName);
    DefaultWeapon = std::shared_ptr<FWeaponInfo>(Info);
    CurrentWeapon = DefaultWeapon;
    // AvailableWeapons.Add(DefaultWeapon);

    auto Stats = std::make_shared<FWeaponStats>();
    Stats->StoredProjectileCount = -1;
    Stats->CurrentProjectileCount = Info->MaxProjectileCount;
    AmmoStorage.Emplace(WeaponName, Stats);
}

std::shared_ptr<FWeaponInfo> UInventoryComponent::CreateNewWeapon(const FName WeaponName) {
    UTopDownGameInstance* GameInctance = Cast<UTopDownGameInstance>(GetOwner()->GetGameInstance());
    if (GameInctance != nullptr) {
        auto WeaponInfo = std::make_shared<FWeaponInfo>();
        if (GameInctance->GetWeaponInfoByName(WeaponName, *WeaponInfo)) {
            return WeaponInfo;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("Weapon [%s] is not created"), *WeaponName.ToString());
    UE_ERROR_MESSAGE("Weapon [%s] is not created", *WeaponName.ToString());
    throw std::runtime_error(
        std::string("Weapon " + std::string(TCHAR_TO_UTF8(*WeaponName.ToString())) + " is not created"));
}

std::shared_ptr<FWeaponStats> UInventoryComponent::GetWeaponStats(const FName WeaponName) {
    auto Stats = AmmoStorage.Find(WeaponName);
    if (Stats != nullptr) {
        return *Stats;
    }

    auto NewWeaponStats = std::make_shared<FWeaponStats>();
    AmmoStorage.Emplace(WeaponName, NewWeaponStats);
    return NewWeaponStats;
}

bool UInventoryComponent::AddWeapon(int32 SlotID, FWeaponInfo&& Info) {
    if (AvailableWeapons.Num() < SlotID + 1) {
        for (size_t i = AvailableWeapons.Num(); i <= SlotID; ++i) {
            AvailableWeapons.Add(nullptr);
        }
    }

    auto Name = Info.WeaponName;

    bool IsWeaponExist = false;
    for (size_t i = 0; i < AvailableWeapons.Num(); ++i) {
        if (AvailableWeapons[i] != nullptr && AvailableWeapons[i]->WeaponName == Name) {
            IsWeaponExist = true;
            SlotID = i;
            break;
        }
    }
    if (!IsWeaponExist) {
        AvailableWeapons[SlotID] = std::make_shared<FWeaponInfo>(Info);
    }

    std::shared_ptr<FWeaponStats> Stats;

    if (AmmoStorage.Contains(Name)) {
        Stats = AmmoStorage[Name];
        auto CountToBeStored = Stats->StoredProjectileCount + Info.MaxProjectileCount;
        if (Info.MaxBulletsCanBeStored <= CountToBeStored) {
            return false;
        }
        Stats->StoredProjectileCount = CountToBeStored;
    } else {
        Stats = std::make_shared<FWeaponStats>();
        Stats->CurrentProjectileCount = Info.MaxProjectileCount;
        AmmoStorage.Emplace(Name, Stats);
    }
    FWeaponSlot NewWeaponSlot;
    NewWeaponSlot.WeaponStats = *Stats;
    NewWeaponSlot.WeaponIcon = Info.WeaponIcon;
    OnWeaponSlotUpdate.Broadcast(false, SlotID, NewWeaponSlot);
    return true;
}

bool UInventoryComponent::AddAmmo(AProjectileDropItem* Item) {
    FName Name = Item->GetWeaponInfo().WeaponName;
    auto ItemStats = Item->GetWeaponStats();
    if (AmmoStorage.Contains(Name)) {
        auto Stats = AmmoStorage[Name];
        auto CountToBeStored = Stats->StoredProjectileCount + ItemStats.StoredProjectileCount;
        if (Item->GetWeaponInfo().MaxBulletsCanBeStored <= CountToBeStored) {
            return false;
        }
        Stats->StoredProjectileCount = CountToBeStored;

        for (size_t i = 0; i < AvailableWeapons.Num(); ++i) {
            if (AvailableWeapons[i] != nullptr && AvailableWeapons[i]->WeaponName == Name) {
                FWeaponSlot NewWeaponSlot;
                NewWeaponSlot.WeaponStats = *Stats;
                NewWeaponSlot.WeaponIcon = Item->GetWeaponInfo().WeaponIcon;
                OnWeaponSlotUpdate.Broadcast(false, i, NewWeaponSlot);
                break;
            }
        }
    } else {
        AmmoStorage.Emplace(Name, std::make_shared<FWeaponStats>(ItemStats));
    }
    return true;
}

bool UInventoryComponent::SwitchCurrentWeaponStats(int SlotID) {
    if (AvailableWeapons.Num() >= SlotID + 1 && AvailableWeapons[SlotID] != nullptr) {
        auto TempSlot = AvailableWeapons[CurrentWeaponID];
        AvailableWeapons[CurrentWeaponID] = AvailableWeapons[SlotID];
        AvailableWeapons[SlotID] = TempSlot;
        if (AvailableWeapons[SlotID] == nullptr) {
            OnWeaponSlotUpdate.Broadcast(false, SlotID, FWeaponSlot());
        } else {
            auto Stats = AmmoStorage[AvailableWeapons[SlotID]->WeaponName];
            FWeaponSlot NewWeaponSlot;
            NewWeaponSlot.WeaponStats = *Stats;
            NewWeaponSlot.WeaponIcon = AvailableWeapons[SlotID]->WeaponIcon;
            OnWeaponSlotUpdate.Broadcast(false, SlotID, NewWeaponSlot);
        }
        CurrentWeapon = AvailableWeapons[CurrentWeaponID];
        FWeaponSlot CurrentWeaponSlot;
        CurrentWeaponSlot.WeaponStats = *AmmoStorage[CurrentWeapon->WeaponName];
        CurrentWeaponSlot.WeaponIcon = CurrentWeapon->WeaponIcon;
        OnWeaponSlotUpdate.Broadcast(false, CurrentWeaponID, CurrentWeaponSlot);
        return true;
    }
    return false;
}

bool UInventoryComponent::SwitchDefaultWeapon() {
    if (AvailableWeapons.Num() > 0) {
        if (CurrentWeapon->WeaponName != DefaultWeapon->WeaponName) {
            FWeaponSlot CurrentWeaponSlot;
            CurrentWeaponSlot.WeaponStats = *AmmoStorage[CurrentWeapon->WeaponName];
            CurrentWeaponSlot.WeaponIcon = CurrentWeapon->WeaponIcon;
            CurrentWeapon = DefaultWeapon;
            OnWeaponSlotUpdate.Broadcast(true, CurrentWeaponID, CurrentWeaponSlot);
        } else if (AvailableWeapons[CurrentWeaponID] != nullptr) {
            CurrentWeapon = AvailableWeapons[CurrentWeaponID];
            FWeaponSlot CurrentWeaponSlot;
            CurrentWeaponSlot.WeaponStats = *AmmoStorage[CurrentWeapon->WeaponName];
            CurrentWeaponSlot.WeaponIcon = CurrentWeapon->WeaponIcon;
            OnWeaponSlotUpdate.Broadcast(true, CurrentWeaponID, CurrentWeaponSlot);
        }
        return true;
    }
    return false;
}
