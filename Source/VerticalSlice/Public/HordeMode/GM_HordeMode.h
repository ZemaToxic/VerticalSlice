// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_HordeMode.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API AGM_HordeMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	AGM_HordeMode();

public:
	// Round Data
	UPROPERTY(BlueprintReadWrite, Category = "Round Data")
		int iCurrentRound;
	UPROPERTY(BlueprintReadWrite, Category = "Round Data")
		int iCurrentScore;
	UPROPERTY(BlueprintReadWrite, Category = "Round Data")
		int iRoundCountdown;
	UPROPERTY(BlueprintReadWrite, Category = "Round Data")
		int iShopCountdown;

	// Game Data
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
		float fPlayerHealthOverride;
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
		float fPlayerDamageOverride;
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
		float fEnemyHealthOverride;
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
		float fEnemyDamageOverride;

private:
	
	void BeginPlay() override;
	void NextRound(int _roundCount);

	void SpawnEnemies(int _enemyCount);
	void SpawnSpecial(int _specialCount);

};

