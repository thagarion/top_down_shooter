// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/ActorComponents/HealthComponent.h"

#include "TopDown/Util/Logger.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
    // features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay() {
    Super::BeginPlay();

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::ApplyDamage);
}

void UHealthComponent::ApplyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                   AController* InstigateBy, AActor* DamageCauser) {
    ReduceHealthValue(Damage);
}

void UHealthComponent::AddHealthValue(float Value) {
    auto NewHealth = CurrentHealth + Value;
    auto HealthAmount = Value;
    if (NewHealth >= MaxHealth) {
        CurrentHealth = MaxHealth;
        HealthAmount = MaxHealth - NewHealth;
    } else {
        CurrentHealth = NewHealth;
    }
    OnHealthChange.Broadcast(MaxHealth, CurrentHealth, ShieldAmount);
    OnTakeHeal.Broadcast(HealthAmount);
}

void UHealthComponent::ReduceHealthValue(float Value) {
    if (!IsInvulnerable) {
        float ShieldDamage = 0.f;
        float HealthDamage = 0.f;
        if (ShieldAmount >= Value) {
            ShieldDamage = Value;
            ShieldAmount -= Value;
        } else {
            ShieldDamage = ShieldAmount;
            HealthDamage = Value - ShieldDamage;
            CurrentHealth -= HealthDamage;
            ShieldAmount = 0;
        }
        OnHealthChange.Broadcast(MaxHealth, CurrentHealth, ShieldAmount);
        OnTakeDamage.Broadcast(HealthDamage + ShieldDamage);
        if (CurrentHealth <= 0) {
            OnDead.Broadcast();
        }
    }
}
