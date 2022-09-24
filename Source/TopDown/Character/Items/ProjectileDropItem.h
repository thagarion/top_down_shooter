// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TopDown/Character/Items/WeaponDropItem.h"

#include "ProjectileDropItem.generated.h"

UCLASS()
class TOPDOWN_API AProjectileDropItem : public AAbstractItem {
    GENERATED_BODY()

protected:
    std::shared_ptr<FWeaponStats> WeaponStats = nullptr;
    std::shared_ptr<FWeaponInfo> WeaponInfo = nullptr;

public:
    // Sets default values for this actor's properties
    AProjectileDropItem();

    void Init(std::shared_ptr<FWeaponInfo> Info, std::shared_ptr<FWeaponStats> Stats);

    UFUNCTION(BlueprintCallable)
    FWeaponStats GetWeaponStats() { return *WeaponStats; }
    UFUNCTION(BlueprintCallable)
    FWeaponInfo GetWeaponInfo() { return *WeaponInfo; }
};
