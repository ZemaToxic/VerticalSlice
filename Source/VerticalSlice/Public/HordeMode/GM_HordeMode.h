// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_HordeMode.generated.h"

class ABaseEnemySpawner;
class AMonsterBase;

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API AGM_HordeMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	AGM_HordeMode();
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABaseEnemySpawner> ClassToFind;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AMonsterBase> EnemyToFind;

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
	UPROPERTY(BlueprintReadWrite, Category = "Round Data")
		int iCurrentEnemies;
	UPROPERTY(BlueprintReadWrite, Category = "Round Data")
		int iInitialEnemies;

	// Game Data
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
		float fPlayerHealthOverride;
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
		float fPlayerDamageOverride;
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
		float fEnemyHealthOverride;
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
		float fEnemyDamageOverride;

	// Timer Data
	UPROPERTY(BlueprintReadWrite, Category = "Timer Data")
		FTimerHandle GameStart;
	UPROPERTY(BlueprintReadWrite, Category = "Timer Data")
		FTimerHandle RoundCooldown;
	UPROPERTY(BlueprintReadWrite, Category = "Timer Data")
		FTimerHandle ShopTimer;

	FTimerHandle tempCooldown;
private:
	
	void BeginPlay() override;
	void StartGame();
	void NextRound(int _roundCount);;

	void SpawnEnemies(int _enemyCount);
	void SpawnSpecial(int _specialCount);

	UFUNCTION(BlueprintCallable)
		void RemoveEnemy();
};

