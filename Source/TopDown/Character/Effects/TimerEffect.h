// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TopDown/Character/Effects/AbstractEffect.h"
#include "TopDown/Character/TopDownCharacter.h"

#include "TimerEffect.generated.h"

UCLASS()
class TOPDOWN_API UTimerEffect : public UAbstractEffect {
    GENERATED_BODY()

protected:
    FTimerHandle Timer;

    int TickCount = 0;

    virtual void Tick();

public:
    // Sets default values for this actor's properties
    UTimerEffect() = default;

    void Init(class ATopDownCharacter* Actor, std::shared_ptr<FEffectInfo> Info) override;
};
