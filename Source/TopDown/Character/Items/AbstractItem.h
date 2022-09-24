// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "AbstractItem.generated.h"

UCLASS()
class TOPDOWN_API AAbstractItem : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AAbstractItem();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class USceneComponent* MeshComponent = nullptr;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class USphereComponent* CollisionSphereComponent = nullptr;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class UProjectileMovementComponent* MovementComponent = nullptr;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class UWidgetComponent* WidgetComponent = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float LifeTime = 60.f;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void CollisionSphereHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor,
                                    class UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
                                    const FHitResult& Hit) {}
    UFUNCTION()
    virtual void CollisionSphereBeginOverlap(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor,
                                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                             bool bFromSweep, const FHitResult& SweepResult) {}
    UFUNCTION()
    virtual void CollisionSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}
};
