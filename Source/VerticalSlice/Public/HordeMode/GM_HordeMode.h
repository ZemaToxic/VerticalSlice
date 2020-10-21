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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void PlayerDead();

	float GetCurrency();
	void SetCurrency(float _newCurrency);
	int GetCurrentEnemies();
	void SetCurrentEnemies();

	// Wave Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Round Data")
		int iCurrentRound;
	UPROPERTY(BlueprintReadOnly, Category = "Wave Data")
		int iCurrentScore;
	UPROPERTY(BlueprintReadOnly, Category = "Wave Data")
		int iCurrentEnemies;
	UPROPERTY(BlueprintReadOnly, Category = "Wave Data")
		int iWaveEnemies;
	UPROPERTY(BlueprintReadOnly, Category = "Wave Data")
		int iInitialEnemies;
	UPROPERTY(BlueprintReadOnly, Category = "Wave Data")
		float fCurrentMoney;
	// Timer Data
	UPROPERTY(BlueprintReadOnly, Category = "Timer Data")
		float fStartTime = 10.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Timer Data")
		float fRoundCooldown = 15.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Timer Data")
		float fShopCooldown = 45.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Timer Data")
		FTimerHandle StartTimer;
	UPROPERTY(BlueprintReadOnly, Category = "Timer Data")
		FTimerHandle RoundTimer;
private:
	UFUNCTION(BlueprintCallable)
		void RemoveEnemy();

	void SetupNextWave();

	void BeginPlay() override;
	void StartGame();
	void NextWave(int _roundCount);

	void BuffPlayer();
	void SpawnEnemies(int _enemyCount, int _enemyType);

	// Game Data
	float fPlayerHealthOverride;
	float fPlayerDamageOverride;
	float fEnemyHealthOverride;
	float fEnemyDamageOverride;

	// Timer Data
	FTimerHandle ShopTimer;
	FTimerHandle PlayerBuff;
};

