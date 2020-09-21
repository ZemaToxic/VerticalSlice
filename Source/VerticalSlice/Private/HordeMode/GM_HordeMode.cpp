// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeMode/GM_HordeMode.h"
#include "HordeMode/BaseEnemySpawner.h"
#include "UObject/ConstructorHelpers.h"


AGM_HordeMode::AGM_HordeMode()
{
	static ConstructorHelpers::FClassFinder<APawn> Blueprint(TEXT("/Game/Blueprints/Player/ThirdPersonCharacter"));
	if (Blueprint.Class != NULL)
	{
		DefaultPawnClass = (UClass*)Blueprint.Class;
	}
}

void AGM_HordeMode::BeginPlay()
{
	// Set Defaults
	iCurrentRound = 1;
	iCurrentScore = 0;
	iRoundCountdown = 45;
	iShopCountdown = 30;
	// Override player Health & Damage
	fPlayerHealthOverride = 100.0f;
	fPlayerDamageOverride = 15.0f;
	// Override base enemy Health & Damage
	fEnemyHealthOverride = 50.0f;
	fEnemyDamageOverride = 10.0f;

	SpawnEnemies(10);
}

void AGM_HordeMode::NextRound(int _roundCount)
{
	
}

void AGM_HordeMode::SpawnEnemies(int _enemyCount)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("GM Spawning Enemies")); }

	FVector CurrentLocation = FVector(0,0,50);
	FRotator CurrentRotation = FRotator(0,0,0);
	FTransform SpawnLocation;
	SpawnLocation.SetLocation(CurrentLocation);
	SpawnLocation.SetRotation(CurrentRotation.Quaternion());

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	UClass* enemyToSpawn = LoadObject<UClass>(this, *FString("Class'/Game/Blueprints/HordeMode/BP_BaseEnemySpawner.BP_BaseEnemySpawner_C'"));

	if (GetWorld())
	{
		ABaseEnemySpawner* newSpawner = GetWorld()->SpawnActor<ABaseEnemySpawner>(enemyToSpawn, SpawnLocation, SpawnInfo);
		newSpawner->SpawnEnemies(_enemyCount);
	}
}

void AGM_HordeMode::SpawnSpecial(int _specialCount)
{
	
}
