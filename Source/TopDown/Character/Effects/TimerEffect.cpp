// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Effects/TimerEffect.h"

#include "TopDown/Util/Logger.h"

inline void UTimerEffect::Tick() {
    if (TickCount <= 0) {
        IsActive = false;
        return;
    }
    TickCount--;
}

void UTimerEffect::Init(class ATopDownCharacter* Actor, std::shared_ptr<FEffectInfo> Info) {
    UAbstractEffect::Init(Actor, Info);
    TickCount = Info->Seconds;
    auto World = CharacterPtr->GetWorld();
    if (World != nullptr) {
        World->GetTimerManager().SetTimer(Timer, this, &UTimerEffect::Tick, 1.f, true);
    } else {
        UE_ERROR_MESSAGE("World is nullptr");
    }
}
