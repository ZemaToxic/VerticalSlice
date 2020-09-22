// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeMode/GM_HordeMode.h"
#include "HordeMode/BaseEnemySpawner.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

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
	iCurrentRound = 1;
	iCurrentScore = 0;
	iRoundCountdown = 45;
	iShopCountdown = 30;
	// Override player Health & Damage.
	fPlayerHealthOverride = 100.0f;
	fPlayerDamageOverride = 15.0f;
	// Override base enemy Health & Damage.
	fEnemyHealthOverride = 50.0f;
	fEnemyDamageOverride = 10.0f;
	// Set Starting Enemy count.
	iCurrentEnemies = 1;
	iInitialEnemies = 8;
	// Start a time to countdown for 30s then Start the game.
	GetWorld()->GetTimerManager().SetTimer(GameStart, this, &AGM_HordeMode::StartGame, 10.0f, true);
}

void AGM_HordeMode::StartGame()
{
	// Start the first round
	NextRound(iCurrentRound);
	// Clear the GameStart timer for memory reasons.
	GetWorld()->GetTimerManager().ClearTimer(GameStart);
}

void AGM_HordeMode::NextRound(int _roundCount)
{
	int enemyCount = (_roundCount * 4) + iInitialEnemies;
	iCurrentEnemies = enemyCount;
	// Spawn the First wave.
	SpawnEnemies(enemyCount);
	iCurrentRound ++;
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
		tempSpawner->SpawnEnemies(_enemyCount/FoundActors.Num());
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
		tempSpawner->SpawnSpecial(iCurrentRound / FoundActors.Num());
	}
}

void AGM_HordeMode::RemoveEnemy()
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("GM Removing Enemy")); }

	iCurrentEnemies--;

	if (iCurrentEnemies <= 0)
	{
		NextRound(iCurrentRound);
	}

}
