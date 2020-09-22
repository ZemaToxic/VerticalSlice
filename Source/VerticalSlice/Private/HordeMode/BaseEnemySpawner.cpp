// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeMode/BaseEnemySpawner.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABaseEnemySpawner::ABaseEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemySpawner::SpawnEnemies(int _enemyCount)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseSpawner Spawning Enemies")); }

	for (int i = 0; i < _enemyCount; i++)
	{
		// Give a random location per Enemy.
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation.X = CurrentLocation.X + FMath::FRandRange(-500.0f, 500.0f);
		CurrentLocation.Y = CurrentLocation.Y + FMath::FRandRange(-1000.0f, 1000.0f);
		CurrentLocation.Z = CurrentLocation.Z + 250.0f;
		// Make sure all enemies start facing the same direction as the spawner.
		FRotator CurrentRotation = GetActorRotation();
		// Conglomerate those two ^ into a FTransform.
		FTransform SpawnLocation;
		SpawnLocation.SetLocation(CurrentLocation);
		SpawnLocation.SetRotation(CurrentRotation.Quaternion());
		// Set Spawn parameters, (Adjust spawn location if colliding with another Actor)
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		// Get the AI Actor to spawn from its Blueprint
		UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprints/Monsters/BP_MonsterA.BP_MonsterA")));
		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		// Double Check the world exists to prevent NULL access errors.
		if (GetWorld())
		{
			// Spawn an enemy with provided information above, and make sure its AIController is enabled.
			AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(GeneratedBP->GeneratedClass, SpawnLocation, SpawnInfo);
			spawnedEnemy->SpawnDefaultController();
			spawnedEnemy->health = 15;
		}
	}
}

void ABaseEnemySpawner::SpawnSpecial(int _enemyCount)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseSpawner Spawning Special")); }

	for (int i = 0; i < _enemyCount; i++)
	{
		// Give a random location per Enemy.
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation.X = CurrentLocation.X + FMath::FRandRange(-250.0f, 250.0f);
		CurrentLocation.Y = CurrentLocation.Y + FMath::FRandRange(-250.0f, 250.0f);
		CurrentLocation.Z = CurrentLocation.Z + 250.0f;
		// Make sure all enemies start facing the same direction as the spawner.
		FRotator CurrentRotation = GetActorRotation();
		// Conglomerate those two ^ into a FTransform.
		FTransform SpawnLocation;
		SpawnLocation.SetLocation(CurrentLocation);
		SpawnLocation.SetRotation(CurrentRotation.Quaternion());
		// Set Spawn parameters, (Adjust spawn location if colliding with another Actor)
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		// Get the AI Actor to spawn from its Blueprint
		UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprints/Monsters/BP_MonsterA.BP_MonsterB")));
		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		// Double Check the world exists to prevent NULL access errors.
		if (GetWorld())
		{
			// Spawn an enemy with provided information above, and make sure its AIController is enabled.
			AMonsterBase* spawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(GeneratedBP->GeneratedClass, SpawnLocation, SpawnInfo);
			spawnedEnemy->SpawnDefaultController();
		}
	}
}
