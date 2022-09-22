// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/TopDownCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"

#include "TopDown/Character/Items/EffectDropItem.h"
#include "TopDown/Character/Items/ProjectileDropItem.h"
#include "TopDown/Character/Items/WeaponDropItem.h"
#include "TopDown/Game/TopDownGameInstance.h"
#include "TopDown/Util/Logger.h"

ATopDownCharacter::ATopDownCharacter() {
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;  // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Create a camera boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true);  // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = CameraHeight;
    CameraBoom->CameraLagSpeed = Smooth;
    CameraBoom->bDoCollisionTest = false;  // Don't want to pull camera in when it collides with level
    CameraBoom->bEnableCameraLag = Flow;
    CameraBoom->SetRelativeRotation(CameraRotation);

    // Create a camera
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false;  // Camera does not rotate relative to arm

    // Create inventory
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

    // Create healht
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATopDownCharacter::BeginPlay() {
    Super::BeginPlay();

    InventoryComponent->SetDefaultWeapon(DefaultWeaponName);
    SetCurrentWeapon();

    if (CursorMaterial) {
        CursorComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
    }

    PlayerControllerPtr = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    // GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ATopDownCharacter::CollisionSphereHit);
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATopDownCharacter::CollisionSphereBeginOverlap);
    GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ATopDownCharacter::CollisionSphereEndOverlap);
}

void ATopDownCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

    if (MovementState != EMovementState::SPRINT_State) {
        if (CursorComponent != nullptr && PlayerControllerPtr != nullptr) {
            FHitResult TraceHitResult;
            PlayerControllerPtr->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
            FVector CursorFV = TraceHitResult.ImpactNormal;
            FRotator CursorR = CursorFV.Rotation();

            CursorComponent->SetWorldLocation(TraceHitResult.Location);
            CursorComponent->SetWorldRotation(CursorR);
        }

        MovementTick(DeltaSeconds);
    } else {
        SprintForward();
    }

    if (CurrentWeapon != nullptr) {
        CurrentWeapon->UpdateStateWeapon(MovementState);
    }

    CameraZoom(DeltaSeconds);
    CameraAimZoom();
}

void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* InComponent) {
    Super::SetupPlayerInputComponent(InComponent);

    InComponent->BindAxis(TEXT("MoveForward"), this, &ATopDownCharacter::MoveForwardInput);
    InComponent->BindAxis(TEXT("MoveRight"), this, &ATopDownCharacter::MoveRightInput);
    InComponent->BindAxis(TEXT("CameraZoom"), this, &ATopDownCharacter::CameraZoomInput);

    // Called from Character BP
    // InComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ATopDownCharacter::FireButtonPressed);
    // InComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &ATopDownCharacter::FireButtonReleased);

    InComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &ATopDownCharacter::AimButtonPressed);
    InComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &ATopDownCharacter::AimButtonReleased);

    InComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ATopDownCharacter::SprintButtonPressed);
    InComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ATopDownCharacter::SprintButtonReleased);

    InComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Pressed, this, &ATopDownCharacter::WalkButtonPressed);
    InComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Released, this, &ATopDownCharacter::WalkButtonReleased);

    InComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &ATopDownCharacter::ReloadButtonPressed);

    InComponent->BindAction<FSlotInput>(TEXT("Slot0"), EInputEvent::IE_Pressed, this,
                                        &ATopDownCharacter::SlotInputButtonPressed, 0);
    InComponent->BindAction<FSlotInput>(TEXT("Slot1"), EInputEvent::IE_Pressed, this,
                                        &ATopDownCharacter::SlotInputButtonPressed, 1);
    InComponent->BindAction<FSlotInput>(TEXT("Slot2"), EInputEvent::IE_Pressed, this,
                                        &ATopDownCharacter::SlotInputButtonPressed, 2);
}

void ATopDownCharacter::ReloadButtonPressed() {
    if (CurrentWeapon != nullptr) {
        CurrentWeapon->Reloading();
    }
}

void ATopDownCharacter::SlotInputButtonPressed(int SlotID) {
    if (IsOverlappedItem && SlotID > 0 &&
        InventoryComponent->AddWeapon(SlotID, std::move(WeaponDropItem->GetWeaponInfo()))) {
        WeaponDropItem->Destroy();
    } else {
        bool IsWeaponSwitched = false;
        if (SlotID > 0) {
            IsWeaponSwitched = InventoryComponent->SwitchCurrentWeaponStats(SlotID);
        } else {
            IsWeaponSwitched = InventoryComponent->SwitchDefaultWeapon();

            /*if (IsWeaponSwitched)
            {
                OnWeaponSwitchDefault.Broadcast();
            }*/
        }
        if (IsWeaponSwitched) {
            CurrentWeapon->StopReloading();
            CurrentWeapon->Destroy();
            SetCurrentWeapon();
        }
    }
}

void ATopDownCharacter::DropRandomWeapon() {
    UTopDownGameInstance* GameInctance = Cast<UTopDownGameInstance>(GetOwner()->GetGameInstance());
    if (GameInctance != nullptr) {
        auto WeaponInfo = std::make_shared<FWeaponInfo>();
        if (GameInctance->GetRandomWeaponInfo(*WeaponInfo) && WeaponInfo->WeaponClass != nullptr) {
            FVector CursorLocation = CursorComponent->GetComponentLocation();
            FVector SpawnLocation = FVector{CursorLocation.X, CursorLocation.Y, 1000.f};
            // + GetActorForwardVector() * 300.f;
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

void ATopDownCharacter::DropRandomProjectile() {
    UTopDownGameInstance* GameInctance = Cast<UTopDownGameInstance>(GetOwner()->GetGameInstance());
    if (GameInctance != nullptr) {
        auto WeaponInfo = std::make_shared<FWeaponInfo>();
        if (GameInctance->GetRandomWeaponInfo(*WeaponInfo) && WeaponInfo->WeaponClass != nullptr) {
            FVector CursorLocation = CursorComponent->GetComponentLocation();
            FVector SpawnLocation = FVector{CursorLocation.X, CursorLocation.Y, 1000.f};
            // + GetActorForwardVector() * 300.f;
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

void ATopDownCharacter::DropRandomEffect() {
    UTopDownGameInstance* GameInctance = Cast<UTopDownGameInstance>(GetOwner()->GetGameInstance());
    if (GameInctance != nullptr) {
        auto EffectInfo = std::make_shared<FEffectInfo>();
        if (GameInctance->GetRandomEffectInfo(*EffectInfo) && EffectInfo->EffectClass != nullptr) {
            FVector CursorLocation = CursorComponent->GetComponentLocation();
            FVector SpawnLocation = FVector{CursorLocation.X, CursorLocation.Y, 1000.f};
            // + GetActorForwardVector() * 300.f;
            FRotator SpawnRotation = GetActorRotation();
            FTransform SpawnTransform = {SpawnRotation, SpawnLocation};
            auto* EffectItem = GetWorld()->SpawnActorDeferred<AEffectDropItem>(EffectInfo->DropClass, SpawnTransform);

            UE_DEBUG_MESSAGE("EffectItem SpawnActorDeferred");
            if (EffectItem != nullptr) {
                UE_DEBUG_MESSAGE("EffectItem != nullptr");
                // auto Stats = std::make_shared<FWeaponStats>();
                // Stats->StoredProjectileCount = FMath::RandRange(1, WeaponInfo->MaxProjectileCount);
                EffectItem->Init(EffectInfo);
                // EffectItem->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
            }
        }
    }
}

void ATopDownCharacter::CollisionSphereBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult) {
    WeaponDropItem = Cast<AWeaponDropItem>(OtherActor);
    if (!IsOverlappedItem && WeaponDropItem != nullptr) {
        IsOverlappedItem = true;
    }

    AProjectileDropItem* OverlappedItem = Cast<AProjectileDropItem>(OtherActor);
    if (OverlappedItem != nullptr && InventoryComponent->AddAmmo(OverlappedItem)) {
        OtherActor->Destroy();
    }
}

void ATopDownCharacter::CollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    auto OverlappedItem = Cast<AWeaponDropItem>(OtherActor);
    if (IsOverlappedItem && OverlappedItem != nullptr) {
        WeaponDropItem = nullptr;
        IsOverlappedItem = false;
    }
}

void ATopDownCharacter::MovementTick(float DeltaTime) {
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f), XAxis);
    AddMovementInput(FVector(0.0f, 1.0f, 0.0f), YAxis);

    if (PlayerControllerPtr != nullptr) {
        FHitResult HitResult;
        PlayerControllerPtr->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult);
        auto Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.Location).Yaw;
        SetActorRotation(FQuat(FRotator(0.0f, Yaw, 0.0f)));

        if (CurrentWeapon != nullptr) {
            CurrentWeapon->SetShootEndLocaion(
                FVector(HitResult.Location.X, HitResult.Location.Y, this->GetActorLocation().Z));
        }
    }
}

void ATopDownCharacter::SprintForward() {
    SetActorRotation(CurrentRotation);
    AddMovementInput(GetActorForwardVector());
}

void ATopDownCharacter::CharacterUpdate() {
    float SpeedResult;
    switch (MovementState) {
        case EMovementState::AIM_State:
            SpeedResult = MovementSpeedInfo.AIM_Speed;
            break;
        case EMovementState::WALK_State:
            SpeedResult = MovementSpeedInfo.WALK_Speed;
            break;
        case EMovementState::RUN_State:
            SpeedResult = MovementSpeedInfo.RUN_Speed;
            break;
        case EMovementState::SPRINT_State:
            SpeedResult = MovementSpeedInfo.SPRINT_Speed;
            break;
        default:
            SpeedResult = MovementSpeedInfo.RUN_Speed;
            break;
    }
    GetCharacterMovement()->MaxWalkSpeed = SpeedResult;
}

void ATopDownCharacter::ChangeMovementState(EMovementState State) {
    MovementState = State;
    CharacterUpdate();

    if (CurrentWeapon != nullptr) {
        CurrentWeapon->UpdateStateWeapon(MovementState);
    }
}

void ATopDownCharacter::ChangeMovementStateOnPressed(EMovementState State) {
    MovementStateStack.emplace_back(State);
    ChangeMovementState(State);
}

void ATopDownCharacter::ChangeMovementStateOnReleased(EMovementState State) {
    if (MovementStateStack.size() > 1) {
        MovementStateStack.erase(std::remove(MovementStateStack.begin(), MovementStateStack.end(), State),
                                 MovementStateStack.end());
    }
    ChangeMovementState(MovementStateStack.back());
}

void ATopDownCharacter::CameraZoomInput(float Value) {
    CameraHeight += CameraZoomStep * Value;

    if (CameraHeight > HeightMax) {
        CameraHeight = HeightMax;
    } else if (CameraHeight < HeightMin) {
        CameraHeight = HeightMin;
    }
}

void ATopDownCharacter::AimButtonPressed() { ChangeMovementStateOnPressed(EMovementState::AIM_State); }

void ATopDownCharacter::AimButtonReleased() {
    if (CameraBoom != nullptr) {
        CameraBoom->SetWorldLocation(GetActorLocation());
    }
    ChangeMovementStateOnReleased(EMovementState::AIM_State);
}

void ATopDownCharacter::CameraZoom(float DeltaTime) {
    if (!FMath::IsNearlyEqual(CameraBoom->TargetArmLength, CameraHeight)) {
        CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, CameraHeight, DeltaTime, 10.0f);
    }
}

void ATopDownCharacter::CameraAimZoom() {
    if (MovementState == EMovementState::AIM_State || MovementState == EMovementState::AIM_WALK_State) {
        const FVector MiddlePoint = (GetActorLocation() + CursorComponent->GetComponentLocation()) / 2;
        CameraBoom->SetWorldLocation(FVector(MiddlePoint.X, MiddlePoint.Y, CameraBoom->GetComponentLocation().Z));
    }
}

void ATopDownCharacter::SetCurrentWeapon() {
    auto WeaponInfo = InventoryComponent->GetCurrentWeaponInfo();
    if (WeaponInfo->WeaponClass != nullptr) {
        FVector SpawnLocation = FVector(0);
        FRotator SpawnRotation = FRotator(0);

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.Owner = GetOwner();
        SpawnParams.Instigator = GetInstigator();

        AAbstractWeapon* Weapon = Cast<AAbstractWeapon>(
            GetWorld()->SpawnActor(WeaponInfo->WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
        if (Weapon != nullptr) {
            FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
            Weapon->AttachToComponent(GetMesh(), Rule, FName("hand_r_weapon"));
            auto WeaponStats = InventoryComponent->GetCurrentWeaponStats();
            Weapon->Init(WeaponInfo, WeaponStats);
            CurrentWeapon = Weapon;
            CurrentWeapon->UpdateStateWeapon(MovementState);
            CurrentWeapon->OnWeaponReloadStart.AddDynamic(this, &ATopDownCharacter::WeaponReloadStart);
            CurrentWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATopDownCharacter::WeaponReloadEnd);
            OnWeaponCreated.Broadcast();
        }
    }
}

void ATopDownCharacter::AttackEvent(bool IsFire) {
    AAbstractWeapon* Weapon = GetCurrentWeapon();
    if (Weapon != nullptr) {
        Weapon->SetWeaponStateFire(IsFire);
    }
}
