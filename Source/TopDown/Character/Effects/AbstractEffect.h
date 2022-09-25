// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TopDown/Character/TopDownCharacter.h"
#include "TopDown/Util/Logger.h"
#include "TopDown/Util/TopDownTypes.h"

#include "AbstractEffect.generated.h"

UCLASS()
class TOPDOWN_API UAbstractEffect : public UObject {
    GENERATED_BODY()

protected:
    bool IsActive = true;
    class ATopDownCharacter* CharacterPtr = nullptr;
    class UHealthComponent* HealthComponentPtr = nullptr;

    std::shared_ptr<FEffectInfo> EffectInfo = nullptr;

public:
    // Sets default values for this actor's properties
    UAbstractEffect() = default;

    virtual void Init(class ATopDownCharacter* Actor, std::shared_ptr<FEffectInfo> Info);

    virtual bool GetIsActive() const { return IsActive; }

    UFUNCTION(BlueprintCallable)
    FEffectInfo GetEffectInfo() const { return *EffectInfo; }

    std::shared_ptr<FEffectInfo> GetEffectInfoPtr() const { return EffectInfo; }
};
