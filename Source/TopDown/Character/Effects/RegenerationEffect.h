// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TopDown/Character/Effects/AbstractEffect.h"

#include "RegenerationEffect.generated.h"

UCLASS()
class TOPDOWN_API ARegenerationEffect : public AAbstractEffect {
    GENERATED_BODY()

protected:
    UHealthComponent* HealthComponentPtr = nullptr;

    float HealthValue = 5.f;
    int Ticks = 10;

public:
    // Sets default values for this actor's properties
    ARegenerationEffect();

    // Called every frame.
    virtual void Tick(float DeltaSeconds) override;

    void Init(ATopDownCharacter* NewActor) override;

};
