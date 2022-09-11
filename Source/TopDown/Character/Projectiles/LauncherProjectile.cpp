// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Projectiles/LauncherProjectile.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "TopDown/Util/Logger.h"

void ALauncherProjectile::BeginPlay() { Super::BeginPlay(); }

void ALauncherProjectile::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ALauncherProjectile::Init(const FProjectileInfo& ProjectileInfo) {
    ProjectileSetting = ProjectileInfo;

    BulletProjectileMovement->InitialSpeed = ProjectileSetting.InitialSpeed;
    BulletProjectileMovement->MaxSpeed = ProjectileSetting.MaxSpeed;

    // ExploseTime = ProjectileSetting.LifeTime;
    GetWorld()->GetTimerManager().SetTimer(Expose_Timer, this, &ALauncherProjectile::Explose,
                                           ProjectileSetting.LifeTime, false);
}

void ALauncherProjectile::Explose() {
    if (ProjectileSetting.ExploseEffect != nullptr) {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExploseEffect, GetActorLocation(),
                                                 GetActorRotation(), FVector(1.0f));
    }
    if (ProjectileSetting.ExploseSound != nullptr) {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.ExploseSound, GetActorLocation());
    }

    TArray<AActor*> IgnoredActor;
    UGameplayStatics::ApplyRadialDamageWithFalloff(
        GetWorld(), ProjectileSetting.Damage, ProjectileSetting.Damage * ProjectileSetting.DamageFactor,
        GetActorLocation(), ProjectileSetting.MaxDamegeRadius, ProjectileSetting.DamegeRadius,
        ProjectileSetting.DamageFalloff, nullptr, IgnoredActor, nullptr, nullptr);
    UE_DEBUG_MESSAGE("ApplyRadialDamageWithFalloff %f", ProjectileSetting.Damage * ProjectileSetting.DamageFactor);

    DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.MaxDamegeRadius, 12, FColor::Orange, false, 3.0f,
                    false, 3.0f);
    DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.DamegeRadius, 12, FColor::Yellow, false, 3.0f,
                    false, 3.0f);

    Destroy();
}