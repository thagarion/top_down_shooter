// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AbstractEffect.generated.h"

UCLASS()
class TOPDOWN_API AAbstractEffect : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AAbstractEffect();

    void SetActor(AActor* NewActor) { ActorPtr = NewActor; };
    virtual void Run(){};

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    AActor* ActorPtr = nullptr;
};
