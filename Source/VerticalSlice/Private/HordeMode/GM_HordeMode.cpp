// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeMode/GM_HordeMode.h"
#include "HordeMode/BaseEnemySpawner.h"
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
	fEnemyHealthOverride = 10.0f;// 50.0f;
	fEnemyDamageOverride = 10.0f;
	// Set Starting Enemy count.
	iCurrentEnemies = 1;
	iInitialEnemies = 8;
	// Start a time to countdown for 30s then Start the game.
	GetWorld()->GetTimerManager().SetTimer(StartTimer, this, &AGM_HordeMode::StartGame, fStartTime, true);
}

void AGM_HordeMode::StartGame()
{
	// Start the first round
	NextRound(iCurrentRound);
	// Clear the GameStart timer for memory reasons.
	GetWorld()->GetTimerManager().ClearTimer(StartTimer);
}

void AGM_HordeMode::NextRound(int _roundCount)
{
	iCurrentRound ++;
	int enemyCount = (_roundCount * 4) + iInitialEnemies;
	iCurrentEnemies = enemyCount;

	if (iCurrentRound % 5 == 0)
	{
		//if (iCurrentRound % 50 == 0)
		//{
		//	iCurrentEnemies = 1;
		//	SpawnBoss(iCurrentEnemies);
		//}
		iCurrentEnemies = 4;	
		// Spawn the special wave.
		SpawnSpecial(iCurrentEnemies);
	}
	else
	{
		// Spawn the First wave.
		SpawnEnemies(enemyCount);
	}
	GetWorld()->GetTimerManager().ClearTimer(RoundTimer);
}

void AGM_HordeMode::SpawnEnemies(int _enemyCount)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("GM Spawning Enemies")); }

	// Find all Monster Spawn locations and Put them in an Array.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
	// Iterate over the FoundActors array.
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		ABaseEnemySpawner* tempSpawner = Cast<ABaseEnemySpawner>(FoundActors[i]); 
		tempSpawner->SpawnEnemies(_enemyCount/FoundActors.Num(), fEnemyHealthOverride, fEnemyDamageOverride);
	}
}

void AGM_HordeMode::SpawnSpecial(int _specialCount)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("GM Spawning Special")); }

	// Find all Monster Spawn locations and Put them in an Array.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
	
	// Iterate over the FoundActors array.
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		ABaseEnemySpawner* tempSpawner = Cast<ABaseEnemySpawner>(FoundActors[i]);
		tempSpawner->SpawnSpecial(iCurrentRound / FoundActors.Num(), fEnemyHealthOverride, fEnemyDamageOverride);
	}
}

void AGM_HordeMode::RemoveEnemy()
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("GM Removing Enemy")); }

	iCurrentEnemies--;

	if (iCurrentEnemies <= 0)
	{
		// Delay the next round to allow shopping 
		FTimerDelegate waveTimer =  FTimerDelegate::CreateUObject(this, &AGM_HordeMode::NextRound, iCurrentRound);
		GetWorld()->GetTimerManager().SetTimer(RoundTimer, waveTimer, fRoundCooldown, true);
	}

}
