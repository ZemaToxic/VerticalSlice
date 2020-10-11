// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeMode/BaseEnemySpawner.h"
#include "Engine/World.h"

// Sets default values
ABaseEnemySpawner::ABaseEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	fSpawnTime = FMath::FRandRange(0.05f, 0.15f);
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
	// Get the current GameMode and Make sure its the HordeMode one.
	CurrentGameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
}

// Called every frame
void ABaseEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemySpawner::SpawnEnemies(int _enemyCount, float _HealthOverride, float _DamageOverride, int _enemyType)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("BaseSpawner Spawning Enemies")); }
	
	// Set local overrides to be used in spawning.
	iEnemiesToSpawn = _enemyCount;
	fEnemyHealth = _HealthOverride;
	fEnemyDamage = _DamageOverride;
	// Change what enemy type is spawned.
	switch (_enemyType)
	{
		case 1: 
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ABaseEnemySpawner::SpawnDefault, fSpawnTime, true);
			break;
		}
		case 2:
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ABaseEnemySpawner::SpawnSecondary, fSpawnTime, true);
			break;
		} 
		case 3:
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ABaseEnemySpawner::SpawnSpecial, fSpawnTime, true);
			break;
		}
		case 4:
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ABaseEnemySpawner::SpawnBoss, fSpawnTime, true);
			break;
		}
		default:
		break;
	}
}

void ABaseEnemySpawner::SpawnDefault()
{
	int CurrentEnemyCount = CurrentGameMode->GetCurrentEnemies();

	// Limit the total amount of enemies on screen at any time.
	if ((CurrentEnemyCount <= 32) && (iEnemiesToSpawn != 0))
	{
		// Give a random location per Enemy.
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation.X = CurrentLocation.X + FMath::FRandRange(-2000.0f, 2000.0f);
		CurrentLocation.Y = CurrentLocation.Y + FMath::FRandRange(-2000.0f, 2000.0f);
		CurrentLocation.Z = CurrentLocation.Z + 250.0f;
		// Add the CurrentLocation to the SpawnLocation FTransform.
		SpawnLocation.SetLocation(CurrentLocation);
		// Double Check the world exists to prevent NULL access errors.
		if (GetWorld())
		{
			// Spawn an enemy with provided information above, and make sure its AIController is enabled.
			AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(mainEnemy, SpawnLocation, SpawnInfo);
			spawnedEnemy->SpawnDefaultController();
			spawnedEnemy->health = fEnemyHealth;
		}
		CurrentGameMode->SetCurrentEnemies();
		iEnemiesToSpawn--;
	}
	else if (iEnemiesToSpawn == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}

void ABaseEnemySpawner::SpawnSecondary()
{
	int CurrentEnemyCount = CurrentGameMode->GetCurrentEnemies();

	if ((CurrentEnemyCount <= 50) && (iEnemiesToSpawn != 0))
	{
		// Give a random location per Enemy.
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation.X = CurrentLocation.X + FMath::FRandRange(-500.0f, 500.0f);
		CurrentLocation.Y = CurrentLocation.Y + FMath::FRandRange(-500.0f, 500.0f);
		CurrentLocation.Z = CurrentLocation.Z + 250.0f;
		// Add the CurrentLocation to the SpawnLocation FTransform.
		SpawnLocation.SetLocation(CurrentLocation);
		// Double Check the world exists to prevent NULL access errors.
		if (GetWorld())
		{
			// Spawn an enemy with provided information above, and make sure its AIController is enabled.
			AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(secondEnemy, SpawnLocation, SpawnInfo);
			spawnedEnemy->SpawnDefaultController();
			spawnedEnemy->health = fEnemyHealth;
		}
		CurrentGameMode->SetCurrentEnemies();
		iEnemiesToSpawn--;
	}
	else if (iEnemiesToSpawn == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}

void ABaseEnemySpawner::SpawnSpecial()
{
	int CurrentEnemyCount = CurrentGameMode->GetCurrentEnemies();

	if ((CurrentEnemyCount <= 50) && (iEnemiesToSpawn != 0))
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
			spawnedEnemy->health = fEnemyHealth;
		}
		CurrentGameMode->SetCurrentEnemies();
		iEnemiesToSpawn--;
	}
	else if (iEnemiesToSpawn == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}

void ABaseEnemySpawner::SpawnBoss()
{
	int CurrentEnemyCount = CurrentGameMode->GetCurrentEnemies();

	if ((CurrentEnemyCount <= 50) && (iEnemiesToSpawn != 0))
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
			AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(bossEnemy, SpawnLocation, SpawnInfo);
			spawnedEnemy->SpawnDefaultController();
			spawnedEnemy->health = fEnemyHealth;
		}
		CurrentGameMode->SetCurrentEnemies();
		iEnemiesToSpawn--;
	}
	else if (iEnemiesToSpawn == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}