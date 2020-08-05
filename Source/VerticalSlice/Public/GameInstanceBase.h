// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "MonsterBase.h"
#include "Mech.h"
#include "GunBase.h"

#include "GameInstanceBase.generated.h"

/**
 * Keeps track of all persistent game data including Parts Collected and Money Collected
 */



UCLASS()
class VERTICALSLICE_API UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TMap<MonsterDrops, int32> partsCollected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float PlayerMoney = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
		MechUpgrades LastMechUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
		AbilityUpgrades LastAbilityUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
		GunUpgrades LastGunUpgrade;

	// Functions

	UFUNCTION(BlueprintCallable)
		void SetData(TMap<MonsterDrops, int32>& _partsCollected, float _PlayerMoney,
			MechUpgrades _LastMechUpgrade, AbilityUpgrades _LastAbilityUpgrade, GunUpgrades _LastGunUpgrade);
};
