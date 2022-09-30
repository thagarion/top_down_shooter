// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <memory>

#include "Components/ActorComponent.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "CoreMinimal.h"

#include "TopDown/Character/Items/ProjectileDropItem.h"
#include "TopDown/Util/Types.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponSlotUpdate, bool, IsWeaponChange, int32, SlotID, FWeaponSlot,
                                               WeaponInfo);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOPDOWN_API UInventoryComponent : public UActorComponent {
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInventoryComponent();

    UPROPERTY(BlueprintAssignable)
    FOnWeaponSlotUpdate OnWeaponSlotUpdate;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    void SetDefaultWeapon(const FName WeaponName);

    std::shared_ptr<FWeaponInfo> CreateNewWeapon(const FName WeaponName);
    std::shared_ptr<FWeaponStats> GetWeaponStats(const FName WeaponName);

    bool AddWeapon(int32 SlotID, FWeaponInfo&& Info);
    bool AddAmmo(AProjectileDropItem* Info);
    bool SwitchCurrentWeaponStats(int SlotID);
    bool SwitchDefaultWeapon();

    std::shared_ptr<FWeaponInfo> GetCurrentWeaponInfo() { return CurrentWeapon; }
    std::shared_ptr<FWeaponStats> GetCurrentWeaponStats() { return GetWeaponStats(CurrentWeapon->WeaponName); }

    UFUNCTION(BlueprintCallable)
    int32 GetCurrentWeaponMaxProjectileCount() { return GetCurrentWeaponInfo()->MaxProjectileCount; }

    UFUNCTION(BlueprintCallable)
    int32 GetAvailableWeaponMaxCount() { return AvailableWeaponMaxCount; }

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    int32 AvailableWeaponMaxCount = 2;

private:
    static const int CurrentWeaponID = 0;

    std::shared_ptr<FWeaponInfo> DefaultWeapon = nullptr;
    std::shared_ptr<FWeaponInfo> CurrentWeapon = nullptr;
    TArray<std::shared_ptr<FWeaponInfo>> AvailableWeapons;
    TMap<FName, std::shared_ptr<FWeaponStats>> AmmoStorage;
};
