// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "FlamethrowerBase.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API AFlamethrowerBase : public AGunBase
{
	GENERATED_BODY()

public:

	virtual void Shoot() override;

	void FlameTick();
	void FlameShoot();

	virtual void Tick(float DeltaTime) override;
};
