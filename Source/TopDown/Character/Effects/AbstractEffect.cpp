// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Effects/AbstractEffect.h"

#include "TopDown/Character/TopDownCharacter.h"

inline void UAbstractEffect::Init(ATopDownCharacter* Actor, std::shared_ptr<FEffectInfo> Info) {
    CharacterPtr = Actor;
    HealthComponentPtr = CharacterPtr->GetHealthComponent();
    EffectInfo = Info;
}
