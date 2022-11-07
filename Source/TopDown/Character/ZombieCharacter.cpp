// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/ZombieCharacter.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "TopDown/Character/TopDownCharacter.h"
#include "TopDown/Util/Logger.h"

// Sets default values
AZombieCharacter::AZombieCharacter() : AAbstractCharacter() {}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay() { Super::BeginPlay(); }

void AZombieCharacter::MovementTick(float DeltaTime) {
    if (PlayerActors.Num() <= 0) {
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopDownCharacter::StaticClass(), PlayerActors);
    }
    if (IsValid(PlayerActors[PlayerIndex])) {
        auto AIController = Cast<AAIController>(GetController());
        if (AIController != nullptr) {
            AIController->MoveToLocation(PlayerActors[PlayerIndex]->GetActorLocation(), 50.f);
        }
    }
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime) {
    if (IsAlive) {
        Super::Tick(DeltaTime);
        MovementTick(DeltaTime);
    }
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AZombieCharacter::Die() {
    if (IsAlive) {
        SetLifeSpan(5.f);

        switch (FMath::RandRange(1, 3)) {
            case 1:
                DropRandomWeapon();
                break;
            case 2:
                DropRandomProjectile();
                break;
            case 3:
                DropRandomEffect();
                break;
        }

        GetCharacterMovement()->StopMovementImmediately();

        return Super::Die();
    }
    return false;
}
