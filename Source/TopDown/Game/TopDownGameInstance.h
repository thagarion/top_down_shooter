// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"

#include "TopDown/Character/Weapons/AbstractWeapon.h"
#include "TopDown/Util/Types.h"

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Setting")
    UDataTable* EffectInfoTable = nullptr;
    UFUNCTION(BlueprintCallable)
    bool GetEffectInfoByName(FName EffectName, FEffectInfo& EffectInfo);
    UFUNCTION(BlueprintCallable)
    bool GetRandomEffectInfo(FEffectInfo& EffectInfo);
};
