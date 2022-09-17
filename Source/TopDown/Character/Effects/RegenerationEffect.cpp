// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Effects/RegenerationEffect.h"

ARegenerationEffect::ARegenerationEffect() {
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 1.f;
}

void ARegenerationEffect::Tick(float DeltaSeconds) {
    if (HealthComponentPtr != nullptr) {
        HealthComponentPtr->AddHealthPercent(HealthValue);
    }
}

void ARegenerationEffect::Init(ATopDownCharacter* NewActor) { 
	AAbstractEffect::Init(NewActor);
    HealthComponentPtr = CharacterPtr->GetHealthComponent();
}
