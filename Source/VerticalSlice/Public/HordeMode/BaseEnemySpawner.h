// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.h" 
#include "GM_HordeMode.h"
#include "BaseEnemySpawner.generated.h"

class AMonsterBase;
class AGM_HordeMode;

UCLASS()
class VERTICALSLICE_API ABaseEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> mainEnemy;	
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> secondEnemy;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> specialEnemy;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> bossEnemy;

	AGM_HordeMode* CurrentGameMode;

	void SpawnEnemies(int _enemyCount, float _HealthOverride, float _DamageOverride, int _enemyType);

private:
	void SpawnDefault();
	void SpawnSecondary();
	void SpawnSpecial();
	void SpawnBoss();

	FTransform SpawnLocation;
	FRotator CurrentRotation;
	FActorSpawnParameters SpawnInfo;

	// Timer Data

	FTimerHandle SpawnTimer;
	float fSpawnTime;

	// Overrides
	int iEnemiesToSpawn;
	float fEnemyHealth;
	float fEnemyDamage;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
