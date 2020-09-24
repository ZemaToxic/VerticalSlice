// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
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
		TSubclassOf<ACharacter> EnemyActor;	
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> secondEnemyActor;

	void SpawnEnemies(int _enemyCount, float _HealthOverride, float _DamageOverride);
	void SpawnSpecial(int _enemyCount, float _HealthOverride, float _DamageOverride);

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
