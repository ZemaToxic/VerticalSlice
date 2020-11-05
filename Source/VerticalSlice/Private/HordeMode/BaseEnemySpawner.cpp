// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeMode/BaseEnemySpawner.h"
#include "Engine/World.h"

// Sets default values
ABaseEnemySpawner::ABaseEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	fSpawnTime = FMath::FRandRange(0.05f, 0.15f);
	eSpawnMode = SpawnMode::singleEnemy;
}

// Called when the game starts or when spawned
void ABaseEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	// Set Spawn parameters, (Adjust spawn location if colliding with another Actor)
	CurrentRotation = GetActorRotation();
	SpawnLocation.SetRotation(CurrentRotation.Quaternion());
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// Get the current GameMode and Make sure its the HordeMode one.
	CurrentGameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
	// Set total max enemies to spawn at once.
	iMaxEnemies = 32;
}

/*
Description: Spawn enemies based on the Enemy type. Start a repeat timer for each enemy type.
Author: Crystal Seymour
*/
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
		case 5:
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ABaseEnemySpawner::SpawnRange, fSpawnTime, true);
			break;
		}
		default:
		{
			// Shouldn't be hit, error out if do
			if (GEngine) 
			{ 
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ERROR no enemy type")); 
			}
			break;
		}

	}
}

void ABaseEnemySpawner::SetSpawnMode(SpawnMode _eSpawnMode)
{
	eSpawnMode = _eSpawnMode;
}

/*
Description: Spawn the main enemy using a random location near the spawner, make sure not to spawn too many at once.
Author: Crystal Seymour
*/
void ABaseEnemySpawner::SpawnDefault()
{
	int CurrentEnemyCount = CurrentGameMode->GetCurrentEnemies();

	// Limit the total amount of enemies on screen at any time.
	if ((CurrentEnemyCount <= iMaxEnemies) && (iEnemiesToSpawn != 0))
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
			AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_mainEnemy, SpawnLocation, SpawnInfo);
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

/*
Description: Spawn the ranged enemy using a random locaiton near the spawner, make sure not to spawn too many at once.
Author: Crystal Seymour
*/
void ABaseEnemySpawner::SpawnSecondary()
{
	int CurrentEnemyCount = CurrentGameMode->GetCurrentEnemies();

	if ((CurrentEnemyCount <= iMaxEnemies) && (iEnemiesToSpawn != 0))
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
			AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_secondEnemy, SpawnLocation, SpawnInfo);
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

/*
Description: Spawn the special enemy using a random location near the spawner, make sure not to spawn too many at once.
Author: Crystal Seymour
*/
void ABaseEnemySpawner::SpawnSpecial()
{
	int CurrentEnemyCount = CurrentGameMode->GetCurrentEnemies();

	if ((CurrentEnemyCount <= iMaxEnemies) && (iEnemiesToSpawn != 0))
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
			AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_specialEnemy, SpawnLocation, SpawnInfo);
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

/*
Description: Spawn the boss enemy using a random location near the spawner, make sure not to spawn too many at once.
Author: Crystal Seymour
*/
void ABaseEnemySpawner::SpawnBoss()
{
	int CurrentEnemyCount = CurrentGameMode->GetCurrentEnemies();

	if ((CurrentEnemyCount <= iMaxEnemies) && (iEnemiesToSpawn != 0))
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
			AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_bossEnemy, SpawnLocation, SpawnInfo);
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



void ABaseEnemySpawner::SpawnRange()
{
	int CurrentEnemyCount = CurrentGameMode->GetCurrentEnemies();
	
	fspawnRange = FMath::FRandRange(0.0f, 1.0f);

	// Limit the total amount of enemies on screen at any time.
	if ((CurrentEnemyCount <= iMaxEnemies) && (iEnemiesToSpawn != 0))
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
			switch (eSpawnMode)
			{
				case singleEnemy: {
					if (fspawnRange < 1.0f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_mainEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					CurrentGameMode->SetCurrentEnemies();
					iEnemiesToSpawn--;
					break;
				}
				case dualEnemies: {
					if (fspawnRange < 0.85f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_mainEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					else if (fspawnRange > 0.75f && fspawnRange < 1.0f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_secondEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					CurrentGameMode->SetCurrentEnemies();
					iEnemiesToSpawn--;
					break;
				}
				case tripleEnemies: {
					if (fspawnRange < 0.7f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_mainEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					else if (fspawnRange > 0.71f && fspawnRange < 0.9f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_secondEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					else if (fspawnRange > 0.91f && fspawnRange < 1.0f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_specialEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					CurrentGameMode->SetCurrentEnemies();
					iEnemiesToSpawn--;
					break;
				}
				case quadEnemies: {
					if (fspawnRange < 0.85f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_mainEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					else if (fspawnRange > 0.86f && fspawnRange < 0.95f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_secondEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					else if (fspawnRange > 0.96f && fspawnRange < 0.98f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_specialEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					else if (fspawnRange > 0.99f && fspawnRange < 1.0f) {
						// Spawn an enemy with provided information above, and make sure its AIController is enabled.
						AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(_bossEnemy, SpawnLocation, SpawnInfo);
						spawnedEnemy->SpawnDefaultController();
						spawnedEnemy->health = fEnemyHealth;
					}
					CurrentGameMode->SetCurrentEnemies();
					iEnemiesToSpawn--;
					break;
				}
				default: {
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ERROR no enemy mode"));
					}
					break;
				}
			}
		}
	}
	else if (iEnemiesToSpawn == 0)
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
		}
	}
}