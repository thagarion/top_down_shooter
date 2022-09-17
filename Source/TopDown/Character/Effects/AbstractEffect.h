// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TopDown/Character/TopDownCharacter.h"
#include "TopDown/Util/Logger.h"

#include "AbstractEffect.generated.h"

UCLASS()
class TOPDOWN_API AAbstractEffect : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AAbstractEffect();

    virtual void Init(ATopDownCharacter* NewActor) { CharacterPtr = NewActor; };

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    ATopDownCharacter* CharacterPtr = nullptr;
};
