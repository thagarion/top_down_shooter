// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Weapons/AbstractWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "TopDown/Character/Items/AbstractItem.h"

// Sets default values
AAbstractWeapon::AAbstractWeapon() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    RootComponent = SceneComponent;

    SkeletalMeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
    SkeletalMeshWeapon->SetGenerateOverlapEvents(false);
    SkeletalMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
    SkeletalMeshWeapon->SetupAttachment(RootComponent);

    StaticMeshWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    StaticMeshWeapon->SetGenerateOverlapEvents(false);
    StaticMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
    StaticMeshWeapon->SetupAttachment(RootComponent);

    ShootLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ShootLocation"));
    ShootLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAbstractWeapon::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AAbstractWeapon::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    FireTick(DeltaTime);
    ReloadTick(DeltaTime);
    DispersionTick(DeltaTime);
}

void AAbstractWeapon::Init(const std::shared_ptr<FWeaponInfo> Info, const std::shared_ptr<FWeaponStats> Stats) {
    if (SkeletalMeshWeapon != nullptr && !SkeletalMeshWeapon->SkeletalMesh) {
        SkeletalMeshWeapon->DestroyComponent(true);
    }

    if (StaticMeshWeapon != nullptr) {
        if (!StaticMeshWeapon->GetStaticMesh()) {
            StaticMeshWeapon->DestroyComponent();
        }
    }

    WeaponSetting = Info;
    WeaponStats = Stats;

    FireRate = WeaponSetting->FireRate;
    ReloadTime = WeaponSetting->ReloadTime;

    FOnWeaponStatChange.Broadcast(*WeaponStats);
}

void AAbstractWeapon::FireTick(float DeltaTime) {
    if (CanFire && !WeaponStats->IsReloading && IsFiring) {
        if (WeaponStats->CurrentProjectileCount > 0) {
            if (FireRate >= WeaponSetting->FireRate) {
                FireRate = 0.0f;
                ChangeDispersion();
                Fire();
            } else {
                FireRate += DeltaTime;
            }
        } else {
            WeaponStats->IsReloading = true;
        }
    }
}

void AAbstractWeapon::ReloadTick(float DeltaTime) {
    if (WeaponStats->IsReloading &&
        (WeaponStats->StoredProjectileCount > 0 || WeaponStats->StoredProjectileCount == -1)) {
        if (ReloadTime > 0.f) {
            if (ReloadTime == WeaponSetting->ReloadTime) {
                OnWeaponReloadStart.Broadcast(WeaponSetting->ReloadTime);
            }
            ReloadTime -= DeltaTime;
        } else {
            WeaponStats->IsReloading = false;
            OnWeaponReloadEnd.Broadcast(true);
            ReloadTime = WeaponSetting->ReloadTime;
            auto AvailableProjectiles =
                FMath::Min(WeaponSetting->MaxProjectileCount, WeaponStats->StoredProjectileCount);
            if (WeaponStats->StoredProjectileCount == -1) {
                AvailableProjectiles = WeaponSetting->MaxProjectileCount;
            } else {
                WeaponStats->StoredProjectileCount -= AvailableProjectiles;
            }
            WeaponStats->CurrentProjectileCount = AvailableProjectiles;
            FOnWeaponStatChange.Broadcast(*WeaponStats);
            if (WeaponSetting->MagazineClass != nullptr) {
                FVector SpawnLocation = GetActorLocation();
                FRotator SpawnRotation = ShootLocation->GetComponentRotation() + FRotator(0.0f, 0.0f, 90.0f);
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                SpawnParams.Owner = GetOwner();
                SpawnParams.Instigator = GetInstigator();
                AAbstractItem* Magazine = Cast<AAbstractItem>(
                    GetWorld()->SpawnActor(WeaponSetting->MagazineClass, &SpawnLocation, &SpawnRotation, SpawnParams));
            }
        }
    }
}

void AAbstractWeapon::DispersionTick(float DeltaTime) {
    if (!WeaponStats->IsReloading) {
        if (!IsFiring) {
            if (ShouldReduceDispersion) {
                CurrentDispersion -= CurrentDispersionReduction;
            } else {
                CurrentDispersion += CurrentDispersionReduction;
            }

            if (CurrentDispersion < CurrentDispersionMin) {
                CurrentDispersion = CurrentDispersionMin;
            } else if (CurrentDispersion > CurrentDispersionMax) {
                CurrentDispersion = CurrentDispersionMax;
            }
        }
    }
}

FVector AAbstractWeapon::ApplyDispersion(FVector Direction) {
    return FMath::VRandCone(Direction, GetCurrentDispersion() * PI / 180.0f);
}

FVector AAbstractWeapon::GetFireEndLocation() {
    FVector EndLocation;
    const FVector ShootLocationVector = ShootLocation->GetComponentLocation();
    const FVector TempVector = (ShootLocationVector - ShootEndLocation);
    if (TempVector.Size() > 100.0f) {
        EndLocation =
            ShootLocationVector + ApplyDispersion((ShootLocationVector - ShootEndLocation).GetSafeNormal()) * -20000.0f;
    } else {
        EndLocation = ShootLocationVector + ApplyDispersion(ShootLocationVector) * 2000;
    }

    return EndLocation;
}

void AAbstractWeapon::SetWeaponStateFire(bool IsFire) {
    if (CanFire) {
        IsFiring = IsFire;
    } else {
        IsFiring = false;
    }
}

void AAbstractWeapon::Fire() {
    if (ShootLocation != nullptr) {
        FVector SpawnLocation = ShootLocation->GetComponentLocation();
        FRotator SpawnRotation = ShootLocation->GetComponentRotation();
        FProjectileInfo ProjectileInfo;
        ProjectileInfo = GetProjectile();

        if (ProjectileInfo.ProjectileClass != nullptr) {
            for (int i = 0; i < WeaponSetting->NumberProjectileByShot; ++i) {
                FVector Direction = GetFireEndLocation() - SpawnLocation;
                Direction.Normalize();

                FMatrix Matrix(Direction, FVector(0, 1, 0), FVector(0, 0, 1), FVector(0, 0, 0));
                SpawnRotation = Matrix.Rotator();

                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                SpawnParams.Owner = GetOwner();
                SpawnParams.Instigator = GetInstigator();
                AAbstractProjectile* Projectile = Cast<AAbstractProjectile>(GetWorld()->SpawnActor(
                    ProjectileInfo.ProjectileClass, &SpawnLocation, &SpawnRotation, SpawnParams));
                if (Projectile != nullptr) {
                    Projectile->Init(WeaponSetting->ProjectileSetting);
                }
            }

            --WeaponStats->CurrentProjectileCount;
            FOnWeaponStatChange.Broadcast(*WeaponStats);

            if (WeaponSetting->SleeveBulletClass != nullptr) {
                FVector SpawnLocationSleeveBullet = GetActorLocation();
                FRotator SpawnRotationSleeveBullet =
                    ShootLocation->GetComponentRotation() + FRotator(0.0f, 0.0f, 90.0f);
                FActorSpawnParameters SpawnParamsSleeveBullet;
                SpawnParamsSleeveBullet.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                SpawnParamsSleeveBullet.Owner = GetOwner();
                SpawnParamsSleeveBullet.Instigator = GetInstigator();
                AAbstractItem* Magazine = Cast<AAbstractItem>(
                    GetWorld()->SpawnActor(WeaponSetting->SleeveBulletClass, &SpawnLocationSleeveBullet,
                                           &SpawnRotationSleeveBullet, SpawnParamsSleeveBullet));
            }

            UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSetting->FireSound,
                                                   ShootLocation->GetComponentLocation());
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponSetting->FireEffect,
                                                     ShootLocation->GetComponentTransform());
        } else {
            FHitResult HitResult = FHitResult(ForceInit);
            FVector TraceEnd = SpawnLocation + (SpawnRotation.Vector() * WeaponSetting->Distance);
            FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
            DrawDebugLine(GetWorld(), SpawnLocation, TraceEnd, FColor::Red, false, 3.0f, false, 3.0f);
            GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation, TraceEnd, ECC_Visibility, TraceParams);
        }
    }
}

void AAbstractWeapon::Reloading() {
    if (WeaponStats->CurrentProjectileCount < WeaponSetting->MaxProjectileCount) {
        WeaponStats->IsReloading = true;
    }
}

void AAbstractWeapon::StopReloading() {
    WeaponStats->IsReloading = false;
    OnWeaponReloadEnd.Broadcast(false);
}

void AAbstractWeapon::UpdateStateWeapon(EMovementState MovementState) {
    CanFire = true;

    switch (MovementState) {
        case EMovementState::AIM_State: {
            CurrentDispersionMax = WeaponSetting->WeaponDispersion.Aim_StateDispersionAimMax;
            CurrentDispersionMin = WeaponSetting->WeaponDispersion.Aim_StateDispersionAimMin;
            CurrentDispersionRecoil = WeaponSetting->WeaponDispersion.Aim_StateDispersionAimRecoil;
            CurrentDispersionReduction = WeaponSetting->WeaponDispersion.Aim_StateDispersionReduction;
            break;
        }
        case EMovementState::AIM_WALK_State: {
            CurrentDispersionMax = WeaponSetting->WeaponDispersion.AimWalk_StateDispersionAimMax;
            CurrentDispersionMin = WeaponSetting->WeaponDispersion.AimWalk_StateDispersionAimMin;
            CurrentDispersionRecoil = WeaponSetting->WeaponDispersion.AimWalk_StateDispersionAimRecoil;
            CurrentDispersionReduction = WeaponSetting->WeaponDispersion.Aim_StateDispersionReduction;
            break;
        }
        case EMovementState::WALK_State: {
            CurrentDispersionMax = WeaponSetting->WeaponDispersion.Walk_StateDispersionAimMax;
            CurrentDispersionMin = WeaponSetting->WeaponDispersion.Walk_StateDispersionAimMin;
            CurrentDispersionRecoil = WeaponSetting->WeaponDispersion.Walk_StateDispersionAimRecoil;
            CurrentDispersionReduction = WeaponSetting->WeaponDispersion.Aim_StateDispersionReduction;
            break;
        }
        case EMovementState::RUN_State: {
            CurrentDispersionMax = WeaponSetting->WeaponDispersion.Run_StateDispersionAimMax;
            CurrentDispersionMin = WeaponSetting->WeaponDispersion.Run_StateDispersionAimMin;
            CurrentDispersionRecoil = WeaponSetting->WeaponDispersion.Run_StateDispersionAimRecoil;
            CurrentDispersionReduction = WeaponSetting->WeaponDispersion.Aim_StateDispersionReduction;
            break;
        }
        case EMovementState::SPRINT_State: {
            CanFire = false;
            break;
        }
        default:
            break;
    }
}

void AAbstractWeapon::ChangeDispersion() { CurrentDispersion += CurrentDispersionRecoil; }
