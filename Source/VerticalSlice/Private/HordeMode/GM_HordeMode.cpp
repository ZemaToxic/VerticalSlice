// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeMode/GM_HordeMode.h"
#include "HordeMode/BaseEnemySpawner.h"
#include "HordeMode/UpgradePedestal.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AGM_HordeMode::AGM_HordeMode()
{
	// Find the Player blueprint 
	static ConstructorHelpers::FClassFinder<APawn> Blueprint(TEXT("/Game/Blueprints/Player/ThirdPersonCharacter"));
	if (Blueprint.Class != NULL)
	{
		// If the player is found, set it as the default player for the GameMode
		DefaultPawnClass = (UClass*)Blueprint.Class;
	}
}

void AGM_HordeMode::BeginPlay()
{
	// Set Defaults.
	iCurrentRound = 0;
	iCurrentScore = 0;
	// Override player Health & Damage.
	fPlayerHealthOverride = 100.0f;
	fPlayerDamageOverride = 15.0f;
	// Override base enemy Health & Damage.
	fEnemyHealthOverride = 50.0f;
	fEnemyDamageOverride = 10.0f;
	// Set Starting Enemy count.
	iWaveEnemies = 0;
	iCurrentEnemies = 1;
	iInitialEnemies = 8;
	// Buff the player initially 
	GetWorld()->GetTimerManager().SetTimer(StartTimer, this, &AGM_HordeMode::BuffPlayer, 0.2f, true);
	// Start a time to countdown for 30s then Start the game.
	GetWorld()->GetTimerManager().SetTimer(StartTimer, this, &AGM_HordeMode::StartGame, fStartTime, true);
}

/*
Description: Increase the players base stats and ammo before the game starts.
Author: Crystal Seymour
*/
void AGM_HordeMode::BuffPlayer()
{
	{
		// Increase the Default Ammo the player has.
		AVerticalSliceCharacter* player = Cast<AVerticalSliceCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (player) {
			player->PlayerMech->UpgradeStats(StatUpgrades::RifleReserveAmmo, 2, true); // 2 = 80 bullets
		}
		GetWorld()->GetTimerManager().ClearTimer(PlayerBuff);
	}
}

/*
Description: Start the game by calling NextWave()
Author: Crystal Seymour
*/
void AGM_HordeMode::StartGame()
{
	// Start the first round
	NextWave(iCurrentRound);
	// Clear the GameStart timer for memory reasons.
	GetWorld()->GetTimerManager().ClearTimer(StartTimer);
}

/*
Description: Determine the amount of enemies to pass into SpawnEnemies()
Author: Crystal Seymour
*/
void AGM_HordeMode::NextWave(int _roundCount)
{
	iCurrentRound ++;
	iCurrentEnemies = 1;
	// Spawn the boss enemy wave
	if (iCurrentRound % 50 == 0)
	{
		iWaveEnemies = 1;
		SpawnEnemies(iWaveEnemies, 4);
	}
	// Spawn the special enemy wave
	else if (iCurrentRound % 25 == 0)
	{
		iWaveEnemies = 4;
		SpawnEnemies(iWaveEnemies, 3);
	}	
	// Spawn the secondary enemy wave.
	else if (iCurrentRound % 5 == 0)
	{
		iWaveEnemies = 4;
		SpawnEnemies(iWaveEnemies, 2);
	}
	// Spawn the First enemy wave.
	else
	{ 
		int enemyCount = (_roundCount * 4) + iInitialEnemies;
		iWaveEnemies = enemyCount;
		SpawnEnemies(iWaveEnemies, 1);
	}
	GetWorld()->GetTimerManager().ClearTimer(RoundTimer);
}

/*
Description: Get the current spawners in the level and pass in the amount of enemies determined in NextWave()
Author: Crystal Seymour
*/
void AGM_HordeMode::SpawnEnemies(int _enemyCount, int _enemyType)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("GM Spawning Enemies")); }

	// Modify Health for the Enemies each wave.
	fEnemyHealthOverride = fEnemyHealthOverride + (iCurrentRound * 5.0f);

	// Find all Monster Spawn locations and Put them in an Array.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), _spawners, FoundActors);
	// Iterate over the FoundActors array.
	
	if (_enemyType != 4)
	{
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			ABaseEnemySpawner* tempSpawner = Cast<ABaseEnemySpawner>(FoundActors[i]); 
			tempSpawner->SpawnEnemies(_enemyCount/FoundActors.Num(), fEnemyHealthOverride, fEnemyDamageOverride, _enemyType);
		}
	}
	else
	{
		int i = FMath::FRandRange(1, FoundActors.Num());
		
		ABaseEnemySpawner* tempSpawner = Cast<ABaseEnemySpawner>(FoundActors[i]);
		tempSpawner->SpawnEnemies(_enemyCount, fEnemyHealthOverride, fEnemyDamageOverride, _enemyType);
	}
}

/*
Description: Get the current amount of Money the player has.
Author: Crystal Seymour
*/
float AGM_HordeMode::GetCurrency()
{
	return fCurrentMoney;
}

/*
Description: Set the current amount of Money the player has.
Author: Crystal Seymour
*/
void AGM_HordeMode::SetCurrency(float _newCurrency)
{
	fCurrentMoney = fCurrentMoney - _newCurrency;
}

/*
Description: Get the current alive enemies currently in the level.
Author: Crystal Seymour
*/
int AGM_HordeMode::GetCurrentEnemies()
{
	return iCurrentEnemies;
}

/*
Description: Increase the current enemy in the level count
Author: Crystal Seymour
*/
void AGM_HordeMode::SetCurrentEnemies()
{
	iCurrentEnemies ++;
}

/*
Description: Remove an Enemy from the current total in the level aswell as wave total.
Author: Crystal Seymour
*/
void AGM_HordeMode::DecrementEnemies()
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("GM Removing Enemy")); }

	iWaveEnemies--;
	iCurrentEnemies--;
	float rewardCurrency = FMath::FRandRange(50.0f, 150.0f);
	fCurrentMoney += rewardCurrency;
	// If no more enemies remaining setup for the next wave.
	if (iWaveEnemies <= 0)
	{
		SetupNextWave();
	}
}

/*
Description: Setup for the next wave by setting the purchasable upgrades
Author: Crystal Seymour
*/
void AGM_HordeMode::SetupNextWave()
{
	// Setup shops 
	// Find all upgrade Pedestals and Put them in an Array.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), _updradePedestals, FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AUpgradePedestal* tempUpgradePedestal = Cast<AUpgradePedestal>(FoundActors[i]);
		tempUpgradePedestal->SetUpgrade();
	}
	// Delay the next round to allow shopping 
	FTimerDelegate waveTimer = FTimerDelegate::CreateUObject(this, &AGM_HordeMode::NextWave, iCurrentRound);
	GetWorld()->GetTimerManager().SetTimer(RoundTimer, waveTimer, fRoundCooldown, true);
}

// TEMPLATE

/*
Description: 
Author: Crystal Seymour
*/