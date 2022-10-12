// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/ZombieCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "TopDown/ActorComponents/HealthComponent.h"
#include "TopDown/Util/Logger.h"

// Sets default values
AZombieCharacter::AZombieCharacter() {
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Create healht
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget"));
    HealthBarWidgetComponent->SetupAttachment(RootComponent);
    HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidgetComponent->SetDrawSize({100.f, 300.f});
   // HealthBarWidgetComponent->AddRelativeLocation({0, 0, 150.f});

    // Set this character to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay() {
    Super::BeginPlay();
    GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AZombieCharacter::CollisionSphereHit);
}

void AZombieCharacter::CollisionSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
                                          const FHitResult& Hit) {
    UE_DEBUG_MESSAGE("AZombieCharacter Received Damage %f", 0);
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
