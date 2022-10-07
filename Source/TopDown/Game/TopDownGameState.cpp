// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/Game/TopDownGameState.h"

bool ATopDownGameState::CanRespawn() { return Lives-- > 0; }