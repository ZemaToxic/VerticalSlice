// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeMode/BaseEnemySpawner.h"
#include "Engine/World.h"

// Sets default values
ABaseEnemySpawner::ABaseEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bEnabled = false;
}

// Called when the game starts or when spawned
void ABaseEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// Make sure all enemies start facing the same direction as the spawner.
	CurrentRotation = GetActorRotation();
	// Add the CurrentLocation to an FTransform.
	SpawnLocation.SetRotation(CurrentRotation.Quaternion());
	// Set Spawn parameters, (Adjust spawn location if colliding with another Actor)
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// Double Check the world exists to prevent NULL access errors.
}

// Called every frame
void ABaseEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemySpawner::SpawnEnemies(int _enemyCount, float _HealthOverride, float _DamageOverride)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseSpawner Spawning Enemies")); }

	if (bEnabled) 
	{
		for (int i = 0; i < _enemyCount; i++)
		{
			// Give a random location per Enemy.
			FVector CurrentLocation = GetActorLocation();
			CurrentLocation.X = CurrentLocation.X + FMath::FRandRange(-500.0f, 500.0f);
			CurrentLocation.Y = CurrentLocation.Y + FMath::FRandRange(-1000.0f, 1000.0f);
			CurrentLocation.Z = CurrentLocation.Z + 250.0f;
			// Add the CurrentLocation to the SpawnLocation FTransform.
			SpawnLocation.SetLocation(CurrentLocation);
			// Double Check the world exists to prevent NULL access errors.
			if (GetWorld())
			{
				// Spawn an enemy with provided information above, and make sure its AIController is enabled.
				AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(mainEnemy, SpawnLocation, SpawnInfo);
				spawnedEnemy->SpawnDefaultController();
				spawnedEnemy->health = _HealthOverride;
			}
		}	
	}
}

void ABaseEnemySpawner::SpawnSecondary(int _enemyCount, float _HealthOverride, float _DamageOverride)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseSpawner Spawning Secondary")); }

	if (bEnabled)
	{
		for (int i = 0; i < _enemyCount; i++)
		{
			// Give a random location per Enemy.
			FVector CurrentLocation = GetActorLocation();
			CurrentLocation.X = CurrentLocation.X + FMath::FRandRange(-250.0f, 250.0f);
			CurrentLocation.Y = CurrentLocation.Y + FMath::FRandRange(-250.0f, 250.0f);
			CurrentLocation.Z = CurrentLocation.Z + 250.0f;
			// Add the CurrentLocation to the SpawnLocation FTransform.
			SpawnLocation.SetLocation(CurrentLocation);
			// Double Check the world exists to prevent NULL access errors.
			if (GetWorld())
			{
				// Spawn an enemy with provided information above, and make sure its AIController is enabled.
				AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(secondEnemy, SpawnLocation, SpawnInfo);
				spawnedEnemy->SpawnDefaultController();
			}
		}
	}
}

void ABaseEnemySpawner::SpawnSpecial(int _enemyCount, float _HealthOverride, float _DamageOverride)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseSpawner Spawning Special")); }

	if (bEnabled)
	{
		for (int i = 0; i < _enemyCount; i++)
		{
			// Give a random location per Enemy.
			FVector CurrentLocation = GetActorLocation();
			CurrentLocation.X = CurrentLocation.X + FMath::FRandRange(-250.0f, 250.0f);
			CurrentLocation.Y = CurrentLocation.Y + FMath::FRandRange(-250.0f, 250.0f);
			CurrentLocation.Z = CurrentLocation.Z + 250.0f;
			// Add the CurrentLocation to the SpawnLocation FTransform.
			SpawnLocation.SetLocation(CurrentLocation);
			// Double Check the world exists to prevent NULL access errors.
			if (GetWorld())
			{
				// Spawn an enemy with provided information above, and make sure its AIController is enabled.
				AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(specialEnemy, SpawnLocation, SpawnInfo);
				spawnedEnemy->SpawnDefaultController();
			}
		}
	}
}

void ABaseEnemySpawner::SpawnBoss(int _enemyCount, float _HealthOverride, float _DamageOverride)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseSpawner Spawning Boss")); }

	if (bEnabled)
	{
		for (int i = 0; i < _enemyCount; i++)
		{
			// Give a random location per Enemy.
			FVector CurrentLocation = GetActorLocation();
			// Add the CurrentLocation to the SpawnLocation FTransform.
			SpawnLocation.SetLocation(CurrentLocation);
			// Double Check the world exists to prevent NULL access errors.
			if (GetWorld())
			{
				// Spawn an enemy with provided information above, and make sure its AIController is enabled.
				AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(bossEnemy, SpawnLocation, SpawnInfo);
				spawnedEnemy->SpawnDefaultController();
			}
		}
	}
}
