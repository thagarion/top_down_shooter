// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "Particles/ParticleSystem.h"

#include "TopDown/Character/Items/AbstractItem.h"
#include "TopDown/Util/TopDownTypes.h"

#include "EffectDropItem.generated.h"

UCLASS()
class TOPDOWN_API AEffectDropItem : public AAbstractItem {
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    UParticleSystem* ParticleSystemCircle;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    UParticleSystem* ParticleSystemEffect;

    std::shared_ptr<FEffectInfo> EffectInfo = nullptr;

public:
    // Sets default values for this actor's properties
    AEffectDropItem();

    void Init(std::shared_ptr<FEffectInfo> Info);

    UFUNCTION(BlueprintCallable)
    FEffectInfo GetEffectStats() { return *EffectInfo; }
};
