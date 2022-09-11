// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChange, float, MaxHealth, float, Health, float, Shield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage, float, HealthDamage, float, ShieldDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOPDOWN_API UHealthComponent : public UActorComponent {
    GENERATED_BODY()

protected:
    float MaxHealth = 100.f;
    float CurrentHealth = MaxHealth;
    float ShieldAmount = MaxHealth * 0.5;

public:
    UPROPERTY(BlueprintAssignable)
    FOnHealthChange OnHealthChange;
    UPROPERTY(BlueprintAssignable)
    FOnTakeDamage OnTakeDamage;
    UPROPERTY(BlueprintAssignable)
    FOnDead OnDead;

public:
    // Sets default values for this component's properties
    UHealthComponent();

    UFUNCTION(BlueprintCallable)
    float GetMaxHealth() { return MaxHealth; }

    UFUNCTION(BlueprintCallable)
    float GetShieldAmount() { return ShieldAmount; }

    UFUNCTION()
    void ApplyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy,
                     AActor* DamageCauser);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
};
