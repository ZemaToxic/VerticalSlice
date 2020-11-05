// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.h" 
#include "GM_HordeMode.h"
#include "BaseEnemySpawner.generated.h"

class AMonsterBase;
class AGM_HordeMode;

enum SpawnMode {
	singleEnemy,
	dualEnemies,
	tripleEnemies,
	quadEnemies
};


UCLASS()
class VERTICALSLICE_API ABaseEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemySpawner();

	void SpawnEnemies(int _enemyCount, float _HealthOverride, float _DamageOverride, int _enemyType);
	void SetSpawnMode(SpawnMode _eSpawnMode);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> _mainEnemy;	
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> _secondEnemy;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> _specialEnemy;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> _bossEnemy;

private:
	AGM_HordeMode* CurrentGameMode;
	FTransform SpawnLocation;
	FRotator CurrentRotation;
	FActorSpawnParameters SpawnInfo;

	// Timer Data
	FTimerHandle SpawnTimer;
	float fSpawnTime;

	// Overrides
	int iMaxEnemies;
	int iEnemiesToSpawn;
	float fEnemyHealth;
	float fEnemyDamage;

	float fspawnRange;
	SpawnMode eSpawnMode;

	void SpawnDefault();
	void SpawnSecondary();
	void SpawnSpecial();
	void SpawnBoss();
	void SpawnRange();
};
