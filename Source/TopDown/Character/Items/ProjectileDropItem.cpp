// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Items/ProjectileDropItem.h"

// Sets default values
AProjectileDropItem::AProjectileDropItem() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
}

void AProjectileDropItem::Init(std::shared_ptr<FWeaponInfo> Info, std::shared_ptr<FWeaponStats> Stats) {
    WeaponInfo = Info;
    WeaponStats = Stats;
}
