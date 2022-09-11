// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"

#include "TopDown/Character/Weapons/AbstractWeapon.h"
#include "TopDown/Util/TopDownTypes.h"

#include "TopDownGameInstance.generated.h"

UCLASS()
class TOPDOWN_API UTopDownGameInstance : public UGameInstance {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Setting")
    UDataTable* WeaponInfoTable = nullptr;
    UFUNCTION(BlueprintCallable)
    bool GetWeaponInfoByName(FName WeaponName, FWeaponInfo& WeaponInfo);
    UFUNCTION(BlueprintCallable)
    bool GetRandomWeaponInfo(FWeaponInfo& WeaponInfo);
};
