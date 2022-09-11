// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Environment/Doors/IceDoorComponent.h"

#include "TopDown/Character/TopDownCharacter.h"

// Sets default Values
AIceDoorComponent::AIceDoorComponent() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
    RootComponent = SceneRoot;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Static Mesh"));
    StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    InitialHeight = StaticMesh->GetRelativeLocation().Z;
    OpenedHeight = InitialHeight - OpeningHeight;

    EnterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
    EnterTrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
    AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called every frame
void AIceDoorComponent::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    DoorMovement(DeltaTime);
    ItemHovering();
}

// Called when the game starts or when spawned
void AIceDoorComponent::BeginPlay() {
    Super::BeginPlay();

    if (SoundCue->IsValidLowLevelFast()) {
        AudioComponent->SetSound(SoundCue);
    }

    EnterTrigger->OnComponentBeginOverlap.AddDynamic(this, &AIceDoorComponent::OnOverlapBegin);
    EnterTrigger->OnComponentEndOverlap.AddDynamic(this, &AIceDoorComponent::OnOverlapEnd);
}

void AIceDoorComponent::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult) {
    if (DoorStatus != EDoorState::OPENING_State) {
        ATopDownCharacter* OtherCharacter = Cast<ATopDownCharacter>(OtherActor);
        if (OtherCharacter != nullptr) {
            DoorStatus = EDoorState::OPENING_State;
            AudioComponent->Play();
        }
    }
}

void AIceDoorComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (DoorStatus != EDoorState::CLOSING_State) {
        ATopDownCharacter* OtherCharacter = Cast<ATopDownCharacter>(OtherActor);
        if (OtherCharacter != nullptr) {
            DoorStatus = EDoorState::CLOSING_State;
            AudioComponent->Play();
        }
    }
}

void AIceDoorComponent::ItemHovering() {
    float Alpha = Rate * FMath::Cos(GetGameTimeSinceCreation());
    FVector Location = FMath::Lerp(Start, End, Alpha);
    StaticMesh->AddRelativeLocation(Location);
}

void AIceDoorComponent::DoorMovement(float DeltaTime) {
    switch (DoorStatus) {
        case EDoorState::OPENING_State: {
            FVector CurrentLocation = StaticMesh->GetRelativeLocation();
            if (CurrentLocation.Z > OpenedHeight) {
                float NewZLocation = FMath::FInterpTo(CurrentLocation.Z, OpenedHeight, DeltaTime, OpeningSpeed);
                StaticMesh->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, NewZLocation));
            } else {
                // DoorStatus = EDoorState::INACTIVE_State;
            }
            break;
        }
        case EDoorState::CLOSING_State: {
            FVector CurrentLocation = StaticMesh->GetRelativeLocation();
            if (CurrentLocation.Z < InitialHeight) {
                float NewZLocation = FMath::FInterpTo(CurrentLocation.Z, InitialHeight, DeltaTime, OpeningSpeed);
                StaticMesh->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, NewZLocation));
            } else {
                // DoorStatus = EDoorState::INACTIVE_State;
            }
            break;
        }
    }
}
