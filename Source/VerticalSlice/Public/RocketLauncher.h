// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "Rocket.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API ARocketLauncher : public AGunBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Rocket")
		TSubclassOf<class ARocket> RocketClass;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Rocket")
		float LaunchDistance = 1000;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Rocket")
		float ExplosionRadius = 500;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Rocket")
		float ExplosionRadiusDefault = 500;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Rocket")
		float ExplosionRadiusUpgrade = 100;
	
public:
	virtual void Shoot() override;

	void ShootRocket();

	void UpgradeExplosionRadius(int _Amount);

	virtual void Tick(float DeltaTime) override;
};
