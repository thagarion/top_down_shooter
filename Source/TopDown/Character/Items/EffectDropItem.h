// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"

#include "TopDown/Character/Items/AbstractItem.h"
#include "TopDown/Util/Logger.h"
#include "TopDown/Util/TopDownTypes.h"

#include "EffectDropItem.generated.h"

UCLASS()
class TOPDOWN_API AEffectDropItem : public AAbstractItem {
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = Particles)
    UParticleSystemComponent* ParticleSystemCircle;
    UPROPERTY(EditDefaultsOnly, Category = Particles)
    UParticleSystemComponent* ParticleSystemEffect;

    std::shared_ptr<FEffectInfo> EffectInfo = nullptr;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

public:
    // Sets default values for this actor's properties
    AEffectDropItem();

    void Init(std::shared_ptr<FEffectInfo> Info);

    UFUNCTION(BlueprintCallable)
    FEffectInfo GetEffectInfo() { return *EffectInfo; }

    std::shared_ptr<FEffectInfo> GetEffectInfoPtr() { return EffectInfo; }
};
