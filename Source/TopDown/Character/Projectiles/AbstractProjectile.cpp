
// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Character/Projectiles/AbstractProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TopDown/Util/Logger.h"

// Sets default values
AAbstractProjectile::AAbstractProjectile() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    BulletCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
    BulletCollisionSphere->SetSphereRadius(16.f);
    BulletCollisionSphere->bReturnMaterialOnMove = true;
    BulletCollisionSphere->SetCanEverAffectNavigation(false);

    RootComponent = BulletCollisionSphere;

    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Projectile Mesh"));
    BulletMesh->SetupAttachment(RootComponent);
    BulletMesh->SetCanEverAffectNavigation(false);

    BulletEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Bullet Effect"));
    BulletEffect->SetupAttachment(RootComponent);

    BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Bullet Projectile Movement"));
    BulletProjectileMovement->UpdatedComponent = RootComponent;
    BulletProjectileMovement->InitialSpeed = 2500.f;
    BulletProjectileMovement->MaxSpeed = 0.f;
    BulletProjectileMovement->bRotationFollowsVelocity = true;
    BulletProjectileMovement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AAbstractProjectile::BeginPlay() {
    Super::BeginPlay();

    BulletCollisionSphere->OnComponentHit.AddDynamic(this, &AAbstractProjectile::BulletCollisionSphereHit);
    BulletCollisionSphere->OnComponentBeginOverlap.AddDynamic(this,
                                                              &AAbstractProjectile::BulletCollisionSphereBeginOverlap);
    BulletCollisionSphere->OnComponentEndOverlap.AddDynamic(this,
                                                            &AAbstractProjectile::BulletCollisionSphereEndOverlap);
}

// Called every frame
void AAbstractProjectile::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AAbstractProjectile::Init(const FProjectileInfo& ProjectileInfo) {
    ProjectileSetting = ProjectileInfo;

    BulletProjectileMovement->InitialSpeed = ProjectileSetting.InitialSpeed;
    BulletProjectileMovement->MaxSpeed = ProjectileSetting.MaxSpeed;

    SetLifeSpan(ProjectileSetting.LifeTime);
}

void AAbstractProjectile::BulletCollisionSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
                                                   const FHitResult& Hit) {
    if (OtherActor && Hit.PhysMaterial.IsValid()) {
        EPhysicalSurface PhysicalSurface = UGameplayStatics::GetSurfaceType(Hit);

        if (ProjectileSetting.HitDecals.Contains(PhysicalSurface)) {
            UMaterialInterface* MaterialInterface = ProjectileSetting.HitDecals[PhysicalSurface];

            if (MaterialInterface != nullptr && OtherComponent != nullptr) {
                UGameplayStatics::SpawnDecalAttached(MaterialInterface, FVector(20.0f), OtherComponent, NAME_None,
                                                     Hit.ImpactPoint, Hit.ImpactNormal.Rotation(),
                                                     EAttachLocation::KeepWorldPosition, 10.0f);
            }
        }
        if (ProjectileSetting.HitEffects.Contains(PhysicalSurface)) {
            UParticleSystem* ParticleSystem = ProjectileSetting.HitEffects[PhysicalSurface];
            if (ParticleSystem != nullptr) {
                UGameplayStatics::SpawnEmitterAtLocation(
                    GetWorld(), ParticleSystem,
                    FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint, FVector(1.0f)));
            }
        }

        if (ProjectileSetting.HitSound != nullptr) {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.HitSound, Hit.ImpactPoint);
        }
    }

    UGameplayStatics::ApplyDamage(OtherActor, ProjectileSetting.Damage, GetInstigatorController(), this, nullptr);
    UE_DEBUG_MESSAGE("ApplyDamage %f", ProjectileSetting.Damage);

    Destroy();
}
