// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Effects/OneTimeEffect.h"
#include "TopDown/Character/TopDownCharacter.h"

void UOneTimeEffect::Init(ATopDownCharacter* Actor, std::shared_ptr<FEffectInfo> Info) {
    UAbstractEffect::Init(Actor, Info);
    auto World = CharacterPtr->GetWorld();
    if (World != nullptr) {
        World->GetTimerManager().SetTimer(Timer, this, &UOneTimeEffect::TimerEnded, Info->Seconds, false);
    } else {
        UE_ERROR_MESSAGE("World is nullptr");
    }
}