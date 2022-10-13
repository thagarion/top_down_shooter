// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/AbstractCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "TopDown/ActorComponents/HealthComponent.h"
#include "TopDown/Character/Effects/AbstractEffect.h"
#include "TopDown/Character/Items/EffectDropItem.h"
#include "TopDown/Character/Items/ProjectileDropItem.h"
#include "TopDown/Character/Items/WeaponDropItem.h"
#include "TopDown/Game/TopDownGameInstance.h"

// Sets default values
AAbstractCharacter::AAbstractCharacter() {
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;  // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Create healht
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

    // Health Widget
    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget"));
    HealthBarWidgetComponent->SetupAttachment(RootComponent);
    HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidgetComponent->SetDrawSize({100.f, 300.f});

    // Set this character to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAbstractCharacter::BeginPlay() {
    Super::BeginPlay();

    GameInctance = Cast<UTopDownGameInstance>(GetOwner()->GetGameInstance());
}

// Called every frame
void AAbstractCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
void AAbstractCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AAbstractCharacter::Die() {
    IsAlive = false;
    float AnimationTime = 0.f;
    auto Index = FMath::RandHelper(DeathMontages.Num());
    if (DeathMontages.IsValidIndex(Index) && DeathMontages[Index] != nullptr) {
        AnimationTime = DeathMontages[Index]->GetPlayLength() * 0.80;
        GetMesh()->GetAnimInstance()->Montage_Play(DeathMontages[Index]);
        GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    }

    GetWorldTimerManager().SetTimer(
        DieAnimationTimer, [&, Index]() { GetMesh()->GetAnimInstance()->Montage_Pause(DeathMontages[Index]); },
        AnimationTime, false);
    return true;
}

void AAbstractCharacter::DropRandomWeapon() {
    if (GameInctance != nullptr) {
        auto WeaponInfo = std::make_shared<FWeaponInfo>();
        if (GameInctance->GetRandomWeaponInfo(*WeaponInfo) && WeaponInfo->WeaponClass != nullptr) {
            FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 100.f);
            FRotator SpawnRotation = GetActorRotation();
            FTransform SpawnTransform = {SpawnRotation, SpawnLocation};
            AWeaponDropItem* WeaponItem =
                GetWorld()->SpawnActorDeferred<AWeaponDropItem>(WeaponInfo->AmmoCrateClass, SpawnTransform);
            if (WeaponItem != nullptr) {
                WeaponItem->Init(WeaponInfo);
                WeaponItem->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
            }
        }
    }
}

void AAbstractCharacter::DropRandomProjectile() {
    if (GameInctance != nullptr) {
        auto WeaponInfo = std::make_shared<FWeaponInfo>();
        if (GameInctance->GetRandomWeaponInfo(*WeaponInfo) && WeaponInfo->WeaponClass != nullptr) {
            FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 100.f);
            FRotator SpawnRotation = GetActorRotation();
            FTransform SpawnTransform = {SpawnRotation, SpawnLocation};
            AProjectileDropItem* ProjectileItem =
                GetWorld()->SpawnActorDeferred<AProjectileDropItem>(WeaponInfo->ProjectileCrateClass, SpawnTransform);
            if (ProjectileItem != nullptr) {
                auto Stats = std::make_shared<FWeaponStats>();
                Stats->StoredProjectileCount = FMath::RandRange(1, WeaponInfo->MaxProjectileCount);
                ProjectileItem->Init(WeaponInfo, Stats);
                ProjectileItem->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
            }
        }
    }
}

void AAbstractCharacter::DropRandomEffect() {
    if (GameInctance != nullptr) {
        auto EffectInfo = std::make_shared<FEffectInfo>();
        if (GameInctance->GetRandomEffectInfo(*EffectInfo) && EffectInfo->EffectClass != nullptr) {
            FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 100.f);
            FRotator SpawnRotation = GetActorRotation();
            FTransform SpawnTransform = {SpawnRotation, SpawnLocation};
            auto* EffectItem = GetWorld()->SpawnActorDeferred<AEffectDropItem>(EffectInfo->DropClass, SpawnTransform);
            if (EffectItem != nullptr) {
                EffectItem->Init(EffectInfo);
                EffectItem->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
            }
        }
    }
}