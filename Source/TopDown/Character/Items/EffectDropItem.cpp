// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Items/EffectDropItem.h"

// Sets default values
AEffectDropItem::AEffectDropItem() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    ParticleSystemCircle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Circle"));
    ParticleSystemCircle->SetupAttachment(RootComponent);
    ParticleSystemEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
    ParticleSystemEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEffectDropItem::BeginPlay() {
    Super::BeginPlay();
    UE_DEBUG_MESSAGE("AEffectDropItem::BeginPlay");
}

// Called every frame
void AEffectDropItem::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    UE_DEBUG_MESSAGE("AEffectDropItem::Tick");
}

void AEffectDropItem::BeginDestroy() {
    Super::BeginDestroy();
    UE_DEBUG_MESSAGE("AEffectDropItem::BeginDestroy");
}

void AEffectDropItem::Init(std::shared_ptr<FEffectInfo> Info) {
    EffectInfo = Info;
    UE_DEBUG_MESSAGE("AEffectDropItem::Init");
}