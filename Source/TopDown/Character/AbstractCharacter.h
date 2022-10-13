// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbstractCharacter.generated.h"

UCLASS()
class TOPDOWN_API AAbstractCharacter : public ACharacter {
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AAbstractCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** Returns HealthComponent subobject **/
    FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

    UFUNCTION(BlueprintCallable)
    virtual bool Die();

protected:
    class UTopDownGameInstance* GameInctance = nullptr;

    FTimerHandle DieAnimationTimer;
    bool IsAlive = true;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void DropRandomWeapon();
    UFUNCTION(BlueprintCallable)
    void DropRandomProjectile();
    UFUNCTION(BlueprintCallable)
    void DropRandomEffect();

private:
    /** Health Component **/
    UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UHealthComponent* HealthComponent = nullptr;

    /** Health Bar Widget **/
    UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UWidgetComponent* HealthBarWidgetComponent = nullptr;

    /** Death Montages **/
    UPROPERTY(EditDefaultsOnly, Category = Montages)
    TArray<class UAnimMontage*> DeathMontages = {};
};
