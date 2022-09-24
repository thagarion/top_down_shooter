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
    MovementComponent->InitialSpeed = 0.f;
    MovementComponent->MaxSpeed = 1000.f;
    MovementComponent->ProjectileGravityScale = 100.f;
    MovementComponent->bRotationFollowsVelocity = true;
    MovementComponent->bShouldBounce = true;
    MovementComponent->bInitialVelocityInLocalSpace = true;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Pick Up Widget"));
    WidgetComponent->SetupAttachment(MeshComponent);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawSize({150.f, 150.f});

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
