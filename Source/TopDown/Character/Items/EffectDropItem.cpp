// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Items/EffectDropItem.h"

// Sets default values
AEffectDropItem::AEffectDropItem() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    ParticleSystemCircle = CreateDefaultSubobject<UParticleSystem>(TEXT("Circle"));
    ParticleSystemEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("Circle"));
}

void AEffectDropItem::Init(std::shared_ptr<FEffectInfo> Info) { EffectInfo = Info; }