// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.h" 
#include "BaseEnemySpawner.generated.h"

class AMonsterBase;

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

	UPROPERTY(EditAnywhere)
		bool bEnabled;

	void SpawnEnemies(int _enemyCount, float _HealthOverride, float _DamageOverride);
	void SpawnSecondary(int _enemyCount, float _HealthOverride, float _DamageOverride);
	void SpawnSpecial(int _enemyCount, float _HealthOverride, float _DamageOverride);
	void SpawnBoss(int _enemyCount, float _HealthOverride, float _DamageOverride);

	FTransform SpawnLocation;
	FRotator CurrentRotation;
	FActorSpawnParameters SpawnInfo;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
