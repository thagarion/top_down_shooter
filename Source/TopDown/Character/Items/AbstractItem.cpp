// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Items/AbstractItem.h"

// Sets default values
AAbstractItem::AAbstractItem() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
    CollisionSphereComponent->SetSphereRadius(16.f);
    CollisionSphereComponent->bReturnMaterialOnMove = true;
    CollisionSphereComponent->SetCanEverAffectNavigation(false);

    RootComponent = CollisionSphereComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCanEverAffectNavigation(false);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Item Movement"));
    MovementComponent->UpdatedComponent = RootComponent;
    MovementComponent->InitialSpeed = 2500.f;
    MovementComponent->MaxSpeed = 0.f;
    MovementComponent->bRotationFollowsVelocity = true;
    MovementComponent->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AAbstractItem::BeginPlay() {
    Super::BeginPlay();

    CollisionSphereComponent->OnComponentHit.AddDynamic(this, &AAbstractItem::CollisionSphereHit);
    CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAbstractItem::CollisionSphereBeginOverlap);
    CollisionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAbstractItem::CollisionSphereEndOverlap);

    SetLifeSpan(LifeTime);
}

// Called every frame
void AAbstractItem::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
