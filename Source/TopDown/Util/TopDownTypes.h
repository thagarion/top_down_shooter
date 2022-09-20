// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TopDownTypes.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8 {
    AIM_State UMETA(DisplayName = "Aim State"),
    AIM_WALK_State UMETA(DisplayName = "Aim Walk State"),
    WALK_State UMETA(DisplayName = "Walk State"),
    RUN_State UMETA(DisplayName = "Run State"),
    SPRINT_State UMETA(DisplayName = "Sprint State"),
};

USTRUCT(BlueprintType)
struct FCharacterSpeed {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AIM_Speed = 100.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WALK_Speed = 400.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RUN_Speed = 600.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SPRINT_Speed = 800.0f;
};

UENUM(BlueprintType)
enum class EDoorState : uint8 {
    OPENING_State UMETA(DisplayName = "Opening State"),
    CLOSING_State UMETA(DisplayName = "Closing State"),
    INACTIVE_State UMETA(DisplayName = "Inactive State"),
    BLOCKED_State UMETA(DisplayName = "Blocked State"),
};

USTRUCT(BlueprintType)
struct FProjectileInfo {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<class AAbstractProjectile> ProjectileClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float Damage = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float DamageFactor = 0.2f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float MaxDamegeRadius = 1000.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float DamegeRadius = 2000.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float DamageFalloff = 5.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float LifeTime = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float InitialSpeed = 2500.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float MaxSpeed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> HitDecals;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> HitEffects;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    UParticleSystem* ExploseEffect = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* HitSound = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* ExploseSound = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponDispersion {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Dispersion ")
    float Aim_StateDispersionAimMax = 2.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Dispersion ")
    float Aim_StateDispersionAimMin = 0.3f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Dispersion ")
    float Aim_StateDispersionAimRecoil = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Dispersion ")
    float Aim_StateDispersionReduction = .3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimWalk Dispersion ")
    float AimWalk_StateDispersionAimMax = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimWalk Dispersion ")
    float AimWalk_StateDispersionAimMin = 0.1f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimWalk Dispersion ")
    float AimWalk_StateDispersionAimRecoil = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimWalk Dispersion ")
    float AimWalk_StateDispersionReduction = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk Dispersion ")
    float Walk_StateDispersionAimMax = 5.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk Dispersion ")
    float Walk_StateDispersionAimMin = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk Dispersion ")
    float Walk_StateDispersionAimRecoil = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk Dispersion ")
    float Walk_StateDispersionReduction = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run Dispersion ")
    float Run_StateDispersionAimMax = 10.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run Dispersion ")
    float Run_StateDispersionAimMin = 4.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run Dispersion ")
    float Run_StateDispersionAimRecoil = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run Dispersion ")
    float Run_StateDispersionReduction = 0.1f;
};

USTRUCT(BlueprintType)
struct FWeaponInfo : public FTableRowBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    FName WeaponName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<class AAbstractWeapon> WeaponClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float FireRate = 0.1f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float ReloadTime = 5.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    int32 MaxProjectileCount = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    int32 NumberProjectileByShot = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    int32 MaxBulletsCanBeStored = MaxProjectileCount * 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    FProjectileInfo ProjectileSetting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
    FWeaponDispersion WeaponDispersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* FireSound = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* ReloadSound = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    UParticleSystem* FireEffect = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    TSubclassOf<class AAbstractItem> MagazineClass = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    TSubclassOf<class AAbstractItem> SleeveBulletClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItem")
    TSubclassOf<class AWeaponDropItem> AmmoCrateClass = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItem")
    TSubclassOf<class AProjectileDropItem> ProjectileCrateClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
    float Damage = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
    float Distance = 2000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    UTexture2D* WeaponIcon = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponStats {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 CurrentProjectileCount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 StoredProjectileCount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    bool IsReloading = false;
};

USTRUCT(BlueprintType)
struct FWeaponSlot {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSlot")
    FWeaponStats WeaponStats;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSlot")
    UTexture2D* WeaponIcon = Cast<UTexture2D>(
        StaticLoadObject(UTexture2D::StaticClass(), NULL,
                         TEXT("/Game/Blueprints/Widgets/Textures/T_Empty_Slot_Icon.T_Empty_Slot_Icon")));
};

USTRUCT(BlueprintType)
struct FEffectInfo : public FTableRowBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    FName EffectName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<class AEffectDropItem> DropClass = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<class AAbstractEffect> EffectClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float Value = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    int Ticks = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    UTexture2D* EffectIcon = nullptr;
};

/*USTRUCT(BlueprintType) 
struct FEffectStats {
    GENERATED_BODY()

};*/

UCLASS()
class TOPDOWN_API UTopDownTypes : public UBlueprintFunctionLibrary {
    GENERATED_BODY()
};
