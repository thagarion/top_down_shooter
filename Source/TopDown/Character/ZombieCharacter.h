// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ZombieCharacter.generated.h"

UCLASS()
class TOPDOWN_API AZombieCharacter : public ACharacter {
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AZombieCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void CollisionSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                            FVector NormalImpulse, const FHitResult& Hit);

private:
    /** Health Component **/
    UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UHealthComponent* HealthComponent = nullptr;

    /** Health Bar Widget **/
    UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UWidgetComponent* HealthBarWidgetComponent = nullptr;
};
