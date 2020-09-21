// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MonsterBase.h"
#include "GunBase.h"

#include "FlamethrowerBase.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API AFlamethrowerBase : public AGunBase
{
	GENERATED_BODY()

private:
	TMap<AMonsterBase*, int> HitMonsters;
	TMap<AMonsterBase*, TPair<int,int>> IgnitedMonsters;

	float FlameTickRate = 0.5f;

	float FlameTickDamage = 5;

	int MaxFlameTicks = 30;
	int MinFlameTicks = 10;

	FTimerHandle FlameTickHandle;

	int HitsToIgnite = 5;
	int MaxHitsToIgnite = 10;
	float ChanceToIgnite = 0.8;

	float FlameRadius = 50;

public:

	virtual void Shoot() override;

	void FlameTick();
	void FlameShoot();

	void IgniteMonster(AMonsterBase* Monster);

	virtual void Tick(float DeltaTime) override;
};
