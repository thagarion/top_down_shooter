// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownGameMode.h"

#include "UObject/ConstructorHelpers.h"

#include "TopDown/Character/TopDownCharacter.h"
#include "TopDown/Game/TopDownPlayerController.h"

ATopDownGameMode::ATopDownGameMode() {
    // use our custom PlayerController class
    PlayerControllerClass = ATopDownPlayerController::StaticClass();

    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_Character"));
    if (PlayerPawnBPClass.Class != NULL) {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}