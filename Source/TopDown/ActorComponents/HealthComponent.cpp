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
    float ShieldDamage = 0.f;
    float HealthDamage = 0.f;
    if (ShieldAmount >= Damage) {
        ShieldDamage = Damage;
        ShieldAmount -= Damage;
    } else {
        ShieldDamage = ShieldAmount;
        HealthDamage = Damage - ShieldDamage;
        CurrentHealth -= HealthDamage;
        ShieldAmount = 0;
    }
    OnHealthChange.Broadcast(MaxHealth, CurrentHealth, ShieldAmount);
    OnTakeDamage.Broadcast(HealthDamage, ShieldAmount);
    if (CurrentHealth <= 0) {
        OnDead.Broadcast();
    }
}

inline void UHealthComponent::AddHealthValue(float Value) {
    auto NewHealth = CurrentHealth + Value;
    if (NewHealth >= MaxHealth) {
        CurrentHealth = MaxHealth;
    } else {
        CurrentHealth = NewHealth;
    }
    OnHealthChange.Broadcast(MaxHealth, CurrentHealth, ShieldAmount);
}
