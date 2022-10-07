// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Templates/Tuple.h"

#include "TopDownGameState.generated.h"

UCLASS()
class TOPDOWN_API ATopDownGameState : public AGameState {
    GENERATED_BODY()

    const int LivesMax = 3;
    int Lives = LivesMax;

public:
    UFUNCTION(BlueprintCallable)
    bool CanRespawn();

    UFUNCTION(BlueprintCallable)
    FORCEINLINE int GetLives() const { return Lives + 1; }
    UFUNCTION(BlueprintCallable)
    FORCEINLINE int GetLivesMax() const { return LivesMax; }
};
