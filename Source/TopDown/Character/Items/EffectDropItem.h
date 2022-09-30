// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"

#include "TopDown/Character/Items/AbstractItem.h"
#include "TopDown/Util/Logger.h"
#include "TopDown/Util/Types.h"
#include "TopDown/Util/Tables.h"
#include "TopDown/Util/Enums.h"

#include "EffectDropItem.generated.h"

UCLASS()
class TOPDOWN_API AEffectDropItem : public AAbstractItem {
    GENERATED_BODY()

protected:
    UPROPERTY(Category = Effects, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UParticleSystemComponent* ParticleSystemCircle;
    UPROPERTY(Category = Effects, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
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
