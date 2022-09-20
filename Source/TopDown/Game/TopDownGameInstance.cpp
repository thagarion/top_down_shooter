// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Game/TopDownGameInstance.h"

#include "Math/UnrealMathUtility.h"

#include "TopDown/Util/Logger.h"
#include "TopDown/Util/TopDownTypes.h"

bool UTopDownGameInstance::GetWeaponInfoByName(FName WeaponName, FWeaponInfo& WeaponInfo) {
    if (WeaponInfoTable == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Table [WeaponInfoTable] not found"));
        UE_ERROR_MESSAGE("Table [WeaponInfoTable] not found");
        return false;
    }
    FWeaponInfo* WeaponInfoOut = WeaponInfoTable->FindRow<FWeaponInfo>(WeaponName, "", false);
    if (WeaponInfoOut == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Weapon [%s] not found in Table"), *WeaponName.ToString());
        UE_ERROR_MESSAGE("Weapon [%s] not found in Table", *WeaponName.ToString());
        return false;
    }
    WeaponInfo = *WeaponInfoOut;
    WeaponInfo.WeaponName = WeaponName;
    return true;
}

bool UTopDownGameInstance::GetRandomWeaponInfo(FWeaponInfo& WeaponInfo) {
    if (WeaponInfoTable == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Table [WeaponInfoTable] not found"));
        UE_ERROR_MESSAGE("Table [WeaponInfoTable] not found");
        return false;
    }
    auto WeaponInfoTableNames = WeaponInfoTable->GetRowNames();
    int32 RandIndex = FMath::RandRange(1, WeaponInfoTableNames.Num() - 1);
    FWeaponInfo* WeaponInfoOut = WeaponInfoTable->FindRow<FWeaponInfo>(WeaponInfoTableNames[RandIndex], "", false);
    if (WeaponInfoOut == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Weapon [%s] not found in Table"), *WeaponInfoTableNames[RandIndex].ToString());
        UE_ERROR_MESSAGE("Weapon [%s] not found in Table", *WeaponInfoTableNames[RandIndex].ToString());
        return false;
    }
    WeaponInfo = *WeaponInfoOut;
    WeaponInfo.WeaponName = WeaponInfoTableNames[RandIndex];
    return true;
}

bool UTopDownGameInstance::GetEffectInfoByName(FName EffectName, FEffectInfo& EffectInfo) {
    if (EffectInfoTable == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Table [EffectInfoTable] not found"));
        UE_ERROR_MESSAGE("Table [EffectInfoTable] not found");
        return false;
    }
    FEffectInfo* EffectInfoOut = EffectInfoTable->FindRow<FEffectInfo>(EffectName, "", false);
    if (EffectInfoOut == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Effect [%s] not found in Table"), *EffectName.ToString());
        UE_ERROR_MESSAGE("Effect [%s] not found in Table", *EffectName.ToString());
        return false;
    }
    EffectInfo = *EffectInfoOut;
    EffectInfo.EffectName = EffectName;
    return true;
}

bool UTopDownGameInstance::GetRandomEffectInfo(FEffectInfo& EffectInfo) {
    if (EffectInfoTable == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Table [EffectInfoTable] not found"));
        UE_ERROR_MESSAGE("Table [EffectInfoTable] not found");
        return false;
    }
    auto EffectInfoTableNames = EffectInfoTable->GetRowNames();
    int32 RandIndex = FMath::RandRange(1, EffectInfoTableNames.Num() - 1);
    FEffectInfo* WeaponInfoOut = EffectInfoTable->FindRow<FEffectInfo>(EffectInfoTableNames[RandIndex], "", false);
    if (WeaponInfoOut == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Weapon [%s] not found in Table"), *EffectInfoTableNames[RandIndex].ToString());
        UE_ERROR_MESSAGE("Weapon [%s] not found in Table", *EffectInfoTableNames[RandIndex].ToString());
        return false;
    }
    EffectInfo = *WeaponInfoOut;
    EffectInfo.EffectName = EffectInfoTableNames[RandIndex];
    return true;
}