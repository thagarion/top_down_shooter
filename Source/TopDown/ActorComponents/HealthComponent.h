// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChange, float, MaxHealth, float, Health, float, Shield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeHeal, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOPDOWN_API UHealthComponent : public UActorComponent {
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = Health)
    float MaxHealth = 0.f;
    UPROPERTY(EditDefaultsOnly, Category = Health)
    float ShieldAmount = 0.f;

    float CurrentHealth = 0.f;

    bool IsInvulnerable = false;
    bool IsAlive = true;

public:
    UPROPERTY(BlueprintAssignable)
    FOnHealthChange OnHealthChange;
    UPROPERTY(BlueprintAssignable)
    FOnTakeDamage OnTakeDamage;
    UPROPERTY(BlueprintAssignable)
    FOnTakeHeal OnTakeHeal;
    UPROPERTY(BlueprintAssignable)
    FOnDead OnDead;

public:
    // Sets default values for this component's properties
    UHealthComponent();

    UFUNCTION(BlueprintCallable)
    float GetMaxHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintCallable)
    float GetShieldAmount() const { return ShieldAmount; }

    UFUNCTION()
    void ApplyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy,
                     AActor* DamageCauser);

    void AddHealthValue(float Value);
    void ReduceHealthValue(float Value);
    void AddHealthPercent(float Value) { AddHealthValue(MaxHealth * Value / 100.f); }
    void ReduceHealthPercent(float Value) { ReduceHealthValue(MaxHealth * Value / 100.f); }

    bool SwitchInvulnerability() {
        IsInvulnerable = !IsInvulnerable;
        return IsInvulnerable;
    }

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
};
