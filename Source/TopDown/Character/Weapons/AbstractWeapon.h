// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <memory>

#include "Components/ArrowComponent.h"
#include "Containers/Map.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TopDown/Character/Projectiles/AbstractProjectile.h"
#include "TopDown/Util/Logger.h"
#include "TopDown/Util/TopDownTypes.h"

#include "AbstractWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponStatChange, FWeaponStats, WeaponStats);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReloadStart, float, ReloadingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReloadEnd, bool, IsSuccessed);

UCLASS()
class TOPDOWN_API AAbstractWeapon : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AAbstractWeapon();

    UPROPERTY(BlueprintAssignable)
    FOnWeaponStatChange FOnWeaponStatChange;
    UPROPERTY(BlueprintAssignable)
    FOnWeaponReloadStart OnWeaponReloadStart;
    UPROPERTY(BlueprintAssignable)
    FOnWeaponReloadEnd OnWeaponReloadEnd;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class USceneComponent* SceneComponent = nullptr;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class USkeletalMeshComponent* SkeletalMeshWeapon = nullptr;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class UStaticMeshComponent* StaticMeshWeapon = nullptr;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    class UArrowComponent* ShootLocation = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Logic")
    bool IsFiring = false;
    UFUNCTION(BlueprintCallable)
    void SetWeaponStateFire(bool bIsFire);
    UFUNCTION(BlueprintCallable)
    void SetShouldReduceDispersion(bool Value) { ShouldReduceDispersion = Value; }

    void Init(const std::shared_ptr<FWeaponInfo> Info, const std::shared_ptr<FWeaponStats> Stats);
    void Reloading();
    void StopReloading();
    void UpdateStateWeapon(EMovementState MovementState);

    UFUNCTION(BlueprintCallable)
    FProjectileInfo GetProjectile() { return WeaponSetting->ProjectileSetting; }
    UFUNCTION(BlueprintCallable)
    FWeaponInfo GetWeaponSetting() { return *WeaponSetting; }

    void SetShootEndLocaion(const FVector& Location) { ShootEndLocation = Location; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    std::shared_ptr<FWeaponInfo> WeaponSetting = nullptr;
    std::shared_ptr<FWeaponStats> WeaponStats = nullptr;

    FVector ShootEndLocation;

    bool ShouldReduceDispersion = false;
    bool CanFire = true;
    float FireRate = 0.0f;
    float ReloadTime = 0.0f;

    float CurrentDispersion = 0.0f;
    float CurrentDispersionMax = 1.0f;
    float CurrentDispersionMin = 0.1f;
    float CurrentDispersionRecoil = 0.1f;
    float CurrentDispersionReduction = 0.1f;

    void ChangeDispersion();
    void Fire();
    void FireTick(float DeltaTime);
    void ReloadTick(float DeltaTime);
    void DispersionTick(float DeltaTime);

    float GetCurrentDispersion() const { return CurrentDispersion; }
    FVector GetFireEndLocation();
    FVector ApplyDispersion(FVector Direction);
};
