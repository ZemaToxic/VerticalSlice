// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

UCLASS()
class VERTICALSLICE_API AGunBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Muzzle", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Muzzle;

	bool Shooting = false;

	float Range = 100.0f;
	
public:	
	// Sets default values for this actor's properties
	AGunBase();

	void Shoot();
	void StopShoot();

protected:

public:

};
