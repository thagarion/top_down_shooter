// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "TopDown/Util/Types.h"

#include "AbstractProjectile.generated.h"

UCLASS()
class TOPDOWN_API AAbstractProjectile : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AAbstractProjectile();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void Init(const FProjectileInfo& ProjectileInfo);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class UStaticMeshComponent* BulletMesh = nullptr;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class USphereComponent* BulletCollisionSphere = nullptr;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class UProjectileMovementComponent* BulletProjectileMovement = nullptr;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class UParticleSystemComponent* BulletEffect = nullptr;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    FProjectileInfo ProjectileSetting;

    UFUNCTION()
    virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor,
                                          class UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
                                          const FHitResult& Hit);
    UFUNCTION()
    virtual void BulletCollisionSphereBeginOverlap(class UPrimitiveComponent* OverlapComponent,
                                                   class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
                                                   int32 OtherBodyIndex, bool bFromSweep,
                                                   const FHitResult& SweepResult) {}
    UFUNCTION()
    virtual void BulletCollisionSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}
};
