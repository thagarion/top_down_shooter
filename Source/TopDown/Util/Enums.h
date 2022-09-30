// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Enums.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8 {
    AIM_State UMETA(DisplayName = "Aim State"),
    AIM_WALK_State UMETA(DisplayName = "Aim Walk State"),
    WALK_State UMETA(DisplayName = "Walk State"),
    RUN_State UMETA(DisplayName = "Run State"),
    SPRINT_State UMETA(DisplayName = "Sprint State"),
};

UENUM(BlueprintType)
enum class EDoorState : uint8 {
    OPENING_State UMETA(DisplayName = "Opening State"),
    CLOSING_State UMETA(DisplayName = "Closing State"),
    INACTIVE_State UMETA(DisplayName = "Inactive State"),
    BLOCKED_State UMETA(DisplayName = "Blocked State"),
};

UENUM(BlueprintType)
enum class EEffectType : uint8 {
    None UMETA(DisplayName = "None"),
    Heal UMETA(DisplayName = "Heal"),
    Regeneration UMETA(DisplayName = "Regeneration"),
    Shield UMETA(DisplayName = "Shield"),
    Shield_Regeneration UMETA(DisplayName = "Shield_Regeneration"),
    Bleeding UMETA(DisplayName = "Bleeding"),
    Invulnerability UMETA(DisplayName = "Invulnerability"),
    Stun UMETA(DisplayName = "Stun"),
};

UCLASS()
class TOPDOWN_API UEnums : public UBlueprintFunctionLibrary {
    GENERATED_BODY()
};
