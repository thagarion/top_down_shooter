// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"

#include "TopDown/Character/TopDownCharacter.h"

ATopDownPlayerController::ATopDownPlayerController() {
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}
