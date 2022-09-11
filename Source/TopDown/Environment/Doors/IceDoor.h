// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TopDown/Environment/Doors/IceDoorComponent.h"

#include "IceDoor.generated.h"

UCLASS()
class TOPDOWN_API AIceDoor : public AActor {
    GENERATED_BODY()

public:
    // Sets default Values for this actor's properties
    AIceDoor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY()
    USceneComponent* SceneRoot;
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    int ItemCount = 3;
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    float ItemSize = 100;
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    TArray<TSubclassOf<AIceDoorComponent>> ItemList;

private:
    void SpawnTwoActors(float& LocationOffset);
    void SpawnRandomActorAndAttach(float LocationOffset);
};
