// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"

#include "TopDown/Util/TopDownTypes.h"

#include "IceDoorComponent.generated.h"

UCLASS()
class TOPDOWN_API AIceDoorComponent : public AActor {
    GENERATED_BODY()

public:
    // Sets default Values for this actor's properties
    AIceDoorComponent();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Items")
    USceneComponent* SceneRoot;
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    float Rate = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    FVector Start = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    FVector End = FVector(0.0f, 0.0f, 0.1f);
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    float OpeningHeight = 300.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    float OpeningSpeed = 3;
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    EDoorState DoorStatus = EDoorState::INACTIVE_State;
    UPROPERTY(EditDefaultsOnly, Category = "Items")
    USoundCue* SoundCue;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* StaticMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* EnterTrigger;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* AudioComponent;

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                        const FHitResult& SweepResult);
    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                      class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    float InitialHeight = 0;
    float OpenedHeight = InitialHeight - OpeningHeight;

    void ItemHovering();
    void DoorMovement(float DeltaTime);
};
