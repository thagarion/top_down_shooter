// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TopDown/Util/Enums.h"
#include "TopDown/Util/Types.h"

#include "Tables.generated.h"

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
struct FEffectInfo : public FTableRowBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    FName EffectName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<class AEffectDropItem> DropClass = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<class UAbstractEffect> EffectClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    EEffectType Type = EEffectType::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float Value = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    int Seconds = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    UTexture2D* EffectIcon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    UParticleSystem* Circle = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    UParticleSystem* Effect = nullptr;
};

USTRUCT(BlueprintType)
struct FLevelInfo : public FTableRowBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    FName MapName = "Coming Soon";
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    bool IsEnabled = false;
};

UCLASS() class TOPDOWN_API UTables : public UBlueprintFunctionLibrary { GENERATED_BODY() };
