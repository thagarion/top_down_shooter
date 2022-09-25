// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TopDown/Character/Effects/AbstractEffect.h"
#include "TopDown/Character/TopDownCharacter.h"

#include "OneTimeEffect.generated.h"

UCLASS()
class TOPDOWN_API UOneTimeEffect : public UAbstractEffect {
    GENERATED_BODY()

protected:
    FTimerHandle Timer;

    virtual void TimerEnded() { UE_WARN_MESSAGE("TimerEnded() not implemented"); };

public:
    // Sets default values for this actor's properties
    UOneTimeEffect() = default;

    void Init(class ATopDownCharacter* Actor, std::shared_ptr<FEffectInfo> Info) override;
};
