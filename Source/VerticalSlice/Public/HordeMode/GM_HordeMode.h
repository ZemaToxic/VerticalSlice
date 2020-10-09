// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_HordeMode.generated.h"

class ABaseEnemySpawner;
class APurchasableDoors;
class AUpgradePedestal;

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
		TSubclassOf<ABaseEnemySpawner> _spawners;
	UPROPERTY(EditAnywhere)
		TSubclassOf<APurchasableDoors> _unlockableDoors;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AUpgradePedestal> _updradePedestals;

public:
	// Round Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Data")
		int iCurrentRound;
	UPROPERTY(BlueprintReadWrite, Category = "Round Data")
		int iCurrentScore;
	UPROPERTY(BlueprintReadWrite, Category = "Round Data")
		int iCurrentEnemies;
	UPROPERTY(BlueprintReadWrite, Category = "Round Data")
		int iWaveEnemies;
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
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
		float fCurrentMoney;

	// Timer Data
	UPROPERTY(BlueprintReadWrite, Category = "Timer Data")
		FTimerHandle StartTimer;
	UPROPERTY(BlueprintReadWrite, Category = "Timer Data")
		FTimerHandle RoundTimer;
	UPROPERTY(BlueprintReadWrite, Category = "Timer Data")
		FTimerHandle ShopTimer;

	UPROPERTY(BlueprintReadWrite, Category = "Timer Data")
		float fStartTime = 10.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Timer Data")
		float fRoundCooldown = 15.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Timer Data")
		float fShopCooldown = 45.0f;

	float GetCurrency();
	int GetCurrentEnemies();
	void SetCurrentEnemies();

private:
	
	void BeginPlay() override;
	void StartGame();
	void NextWave(int _roundCount);;

	void SpawnEnemies(int _enemyCount, int _enemyType);

	UFUNCTION(BlueprintCallable)
		void RemoveEnemy();
};

