// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <memory>

#include "CoreMinimal.h"

#include "TopDown/Character/Items/AbstractItem.h"
#include "TopDown/Util/Logger.h"
#include "TopDown/Util/TopDownTypes.h"

#include "WeaponDropItem.generated.h"

UCLASS()
class TOPDOWN_API AWeaponDropItem : public AAbstractItem {
    GENERATED_BODY()

protected:
    std::shared_ptr<FWeaponStats> WeaponStats = nullptr;
    std::shared_ptr<FWeaponInfo> WeaponInfo = nullptr;

public:
    // Sets default values for this actor's properties
    AWeaponDropItem();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void Init(std::shared_ptr<FWeaponInfo> Info);

    UFUNCTION(BlueprintCallable)
    FWeaponInfo GetWeaponInfo() { return *WeaponInfo; }
};
