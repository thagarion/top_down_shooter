// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TopDown/Character/Projectiles/AbstractProjectile.h"

#include "LauncherProjectile.generated.h"

UCLASS()
class TOPDOWN_API ALauncherProjectile : public AAbstractProjectile {
    GENERATED_BODY()

private:
    FTimerHandle Expose_Timer;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor,
                                          class UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
                                          const FHitResult& Hit) {}

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Init(const FProjectileInfo& ProjectileInfo) override;

    void Explose();
};
