// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TopDown/Character/Items/AbstractItem.h"
#include "TopDown/Util/TopDownTypes.h"

#include "EffectDropItem.generated.h"

UCLASS()
class TOPDOWN_API AEffectDropItem : public AAbstractItem {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AEffectDropItem(){};

    // void Init(std::shared_ptr<FEffectInfo> Info, std::shared_ptr<FWeaponStats> Stats);

    // UFUNCTION(BlueprintCallable)
    // FWeaponStats GetEffectStats() { }
};
