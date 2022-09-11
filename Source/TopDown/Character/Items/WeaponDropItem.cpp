// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Items/WeaponDropItem.h"

#include "TopDown/Character/Weapons/AbstractWeapon.h"

// Sets default values
AWeaponDropItem::AWeaponDropItem() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWeaponDropItem::BeginPlay() {
    Super::BeginPlay();

    LifeTime = 60.f;
    SetLifeSpan(LifeTime);
}

// Called every frame
void AWeaponDropItem::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AWeaponDropItem::Init(std::shared_ptr<FWeaponInfo> Info) { WeaponInfo = Info; }
