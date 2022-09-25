// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TopDown/Character/Effects/TimerEffect.h"

#include "BleedingEffect.generated.h"

UCLASS()
class TOPDOWN_API UBleedingEffect : public UTimerEffect {
    GENERATED_BODY()

protected:
    float Value = 0.f;

    virtual void Tick() override {
        UTimerEffect::Tick();
        if (IsActive) {
            HealthComponentPtr->ReduceHealthValue(Value);
        }
    }

public:
    void Init(class ATopDownCharacter* Actor, std::shared_ptr<FEffectInfo> Info) override {
        UTimerEffect::Init(Actor, Info);
        Value = Info->Value / Info->Seconds;
    }
};
