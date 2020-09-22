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
	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Monster Lists", meta = (AllowPrivateAccess = "true"))
		TMap<AMonsterBase*, int> HitMonsters;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables |	Monster Lists", meta = (AllowPrivateAccess = "true"))
		TMap<AMonsterBase*, TPair<int,int>> IgnitedMonsters;


	UPROPERTY(EditAnywhere, Category = "CustomVariables | Fire Damage", meta = (AllowPrivateAccess = "true"))
		float FlameTickRate = 0.2f;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Fire Damage", meta = (AllowPrivateAccess = "true"))
		float FlameTickDamage = 1;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Upgrades", meta = (AllowPrivateAccess = "true"))
		float FlameTickDamageIncrement = 1;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Fire Damage", meta = (AllowPrivateAccess = "true"))
		float DefaultFlameTickDamage = 1;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Fire Damage", meta = (AllowPrivateAccess = "true"))
		int MaxFlameTicks = 30;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Fire Damage", meta = (AllowPrivateAccess = "true"))
		int MinFlameTicks = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Fire Damage", meta = (AllowPrivateAccess = "true"))
		FTimerHandle FlameTickHandle;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Ignition", meta = (AllowPrivateAccess = "true"))
		int MinHitsToIgnite = 5;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Ignition", meta = (AllowPrivateAccess = "true"))
		int MaxHitsToIgnite = 10;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Ignition", meta = (AllowPrivateAccess = "true"))
		float ChanceToIgnite = 0.8;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Range", meta = (AllowPrivateAccess = "true"))
		float FlameRadius = 50;

public:

	virtual void Shoot() override;

	virtual void StopShoot() override;

	void FlameTick();
	void FlameShoot();

	void UpgradeFireDamage(int _Amount);

	void IgniteMonster(AMonsterBase* Monster);

	virtual void Tick(float DeltaTime) override;
};
