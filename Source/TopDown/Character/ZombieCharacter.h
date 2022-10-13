// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "TopDown/Character/AbstractCharacter.h"

#include "ZombieCharacter.generated.h"

UCLASS()
class TOPDOWN_API AZombieCharacter : public AAbstractCharacter {
    GENERATED_BODY()

private:
    TArray<AActor*> PlayerActors;
    int32 PlayerIndex = 0;

public:
    // Sets default values for this character's properties
    AZombieCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    bool Die() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void MovementTick(float DeltaTime);
};
