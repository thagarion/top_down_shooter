// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Environment/Doors/IceDoor.h"

// Sets default Values
AIceDoor::AIceDoor() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
    RootComponent = SceneRoot;
}

// Called when the game starts or when spawned
void AIceDoor::BeginPlay() {
    Super::BeginPlay();

    float LocationOffset = 0;
    int FirstIndex = 0;
    if (ItemCount % 2 == 1) {
        SpawnRandomActorAndAttach(LocationOffset);
        FirstIndex = 1;
        LocationOffset += ItemSize;
    } else {
        LocationOffset = ItemSize / 2;
    }
    for (int Index = FirstIndex; Index <= ItemCount / 2; ++Index) {
        SpawnTwoActors(LocationOffset);
    }
}

void AIceDoor::SpawnTwoActors(float& LocationOffset) {
    SpawnRandomActorAndAttach(LocationOffset);
    SpawnRandomActorAndAttach(-LocationOffset);
    LocationOffset += ItemSize;
}

void AIceDoor::SpawnRandomActorAndAttach(float LocationOffset) {
    int32_t ItemListCount = ItemList.Num();
    if (ItemListCount > 0) {
        int32_t Index = FMath::RandRange(1, ItemListCount) - 1;
        AIceDoorComponent* ItemActor =
            GetWorld()->SpawnActor<AIceDoorComponent>(ItemList[Index], FVector::ZeroVector, FRotator::ZeroRotator);
        if (IsValid(ItemActor)) {
            ItemActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            ItemActor->AddActorLocalOffset(FVector(0, LocationOffset, 0));
        }
    }
}
