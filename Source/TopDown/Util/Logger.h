// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define UE_DEBUG_MESSAGE(x, ...) if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT(x), __VA_ARGS__)); }
#define UE_INFO_MESSAGE(x, ...) if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT(x), __VA_ARGS__)); }
#define UE_WARN_MESSAGE(x, ...) if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, FString::Printf(TEXT(x), __VA_ARGS__)); }
#define UE_ERROR_MESSAGE(x, ...) if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__)); }
#define UE_FATAL_MESSAGE(x, ...) if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, FString::Printf(TEXT(x), __VA_ARGS__)); }