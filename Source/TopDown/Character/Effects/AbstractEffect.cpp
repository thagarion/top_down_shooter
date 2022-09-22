// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Effects/AbstractEffect.h"

// Sets default values
AAbstractEffect::AAbstractEffect() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AAbstractEffect::BeginPlay() { Super::BeginPlay(); }
