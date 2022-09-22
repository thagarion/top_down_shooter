// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <Components/DecalComponent.h>
#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include <algorithm>
#include <vector>

#include "TopDown/ActorComponents/HealthComponent.h"
#include "TopDown/ActorComponents/InventoryComponent.h"
#include "TopDown/Character/Effects/AbstractEffect.h"
#include "TopDown/Character/Weapons/AbstractWeapon.h"
#include "TopDown/Util/Logger.h"
#include "TopDown/Util/TopDownTypes.h"

#include "TopDownCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponCreated);
DECLARE_DELEGATE_OneParam(FSlotInput, int);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponSwitchDefault);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitch, FWeaponSlot, WeaponInfo);

UCLASS(Blueprintable)
class ATopDownCharacter : public ACharacter {
    GENERATED_BODY()

public:
    ATopDownCharacter();

    UPROPERTY(BlueprintAssignable)
    FOnWeaponCreated OnWeaponCreated;
    UPROPERTY(BlueprintAssignable)
    FOnWeaponSwitchDefault OnWeaponSwitchDefault;
    UPROPERTY(BlueprintAssignable)
    FOnWeaponSwitch OnWeaponSwitch;

    // Called every frame.
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* InComponent) override;
    virtual void BeginPlay() override;

    /** Returns TopDownCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns CursorToWorld subobject **/
    FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorComponent; }
    /** Returns InventoryComponent subobject **/
    FORCEINLINE class UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
    /** Returns HealthComponent subobject **/
    FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float HeightMax = 2000.0f;
    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float HeightMin = 300.0f;
    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float Smooth = 5.0f;
    UPROPERTY(EditDefaultsOnly, Category = Camera)
    bool Flow = false;
    UPROPERTY(EditDefaultsOnly, Category = Camera)
    FRotator CameraRotation = {-75.f, 0.f, 0.f};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
    EMovementState MovementState = EMovementState::RUN_State;
    UPROPERTY(EditDefaultsOnly, Category = Movement)
    FCharacterSpeed MovementSpeedInfo;
    UPROPERTY(EditDefaultsOnly, Category = Cursor)
    UMaterialInterface* CursorMaterial = nullptr;
    UPROPERTY(EditDefaultsOnly, Category = Cursor)
    FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    FName DefaultWeaponName = "Pistol";

    UFUNCTION(BlueprintCallable)
    bool GetIsReloading() { return IsReloading; }
    UFUNCTION(BlueprintCallable)
    EMovementState GetMovementState() { return MovementState; }
    UFUNCTION(BlueprintCallable)
    AAbstractWeapon* GetCurrentWeapon() { return CurrentWeapon; }
    UFUNCTION(BlueprintCallable)
    void SetCurrentWeapon();
    UFUNCTION(BlueprintCallable)
    void AttackEvent(bool IsFire);
    UFUNCTION(BlueprintCallable)
    void FireButtonPressed() { AttackEvent(true); }
    UFUNCTION(BlueprintCallable)
    void FireButtonReleased() { AttackEvent(false); }
    UFUNCTION(BlueprintCallable)
    void ReloadButtonPressed();
    UFUNCTION(BlueprintCallable)
    void SlotInputButtonPressed(int SlotID);
    UFUNCTION()
    void WeaponReloadStart(float ReloadingTime) { IsReloading = true; }
    UFUNCTION()
    void WeaponReloadEnd(bool IsSuccessed) { IsReloading = false; }

    // Only for Debug
    UFUNCTION(BlueprintCallable)
    void DropRandomWeapon();
    UFUNCTION(BlueprintCallable)
    void DropRandomProjectile();
    UFUNCTION(BlueprintCallable)
    void DropRandomEffect();

protected:
    UFUNCTION()
    virtual void CollisionSphereBeginOverlap(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor,
                                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                             bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    virtual void CollisionSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    /** Top down camera */
    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** Inventory Component **/
    UPROPERTY(Category = Inventory, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UInventoryComponent* InventoryComponent;

    /** Health Component **/
    UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UHealthComponent* HealthComponent;

    float XAxis = 0.0f;
    float YAxis = 0.0f;

    float XMouse = 0.0f;
    float YMouse = 0.0f;

    FRotator CurrentRotation;

    class UDecalComponent* CursorComponent = nullptr;

    AAbstractWeapon* CurrentWeapon = nullptr;
    AWeaponDropItem* WeaponDropItem = nullptr;

    float CameraHeight = HeightMax;
    float CameraZoomStep = 50.0f;
    FVector CameraInitialLocation;

    bool IsReloading = false;

    bool IsOverlappedItem = false;

    std::vector<EMovementState> MovementStateStack = {EMovementState::RUN_State};

    APlayerController* PlayerControllerPtr = nullptr;

    void MoveForwardInput(float Value) { XAxis = Value; }
    void MoveRightInput(float Value) { YAxis = Value; }
    void CameraZoomInput(float Value);

    void AimButtonPressed();
    void AimButtonReleased();

    void SprintButtonPressed() {
        if (CursorComponent != nullptr) {
            CursorComponent->SetHiddenInGame(true);
        }
        if (PlayerControllerPtr != nullptr) {
            PlayerControllerPtr->GetMousePosition(XMouse, YMouse);
        }
        CurrentRotation = GetActorRotation();
        ChangeMovementStateOnPressed(EMovementState::SPRINT_State);
    }
    void SprintButtonReleased() {
        if (CursorComponent != nullptr) {
            CursorComponent->SetHiddenInGame(false);
        }
        if (PlayerControllerPtr != nullptr) {
            PlayerControllerPtr->SetMouseLocation(XMouse, YMouse);
        }
        ChangeMovementStateOnReleased(EMovementState::SPRINT_State);
    }

    void WalkButtonPressed() { ChangeMovementStateOnPressed(EMovementState::WALK_State); }
    void WalkButtonReleased() { ChangeMovementStateOnReleased(EMovementState::WALK_State); }

    void MovementTick(float DeltaTime);
    void SprintForward();
    void CharacterUpdate();

    void CameraZoom(float Value);
    void CameraAimZoom();

    void ChangeMovementState(EMovementState State);
    void ChangeMovementStateOnPressed(EMovementState State);
    void ChangeMovementStateOnReleased(EMovementState State);
};
