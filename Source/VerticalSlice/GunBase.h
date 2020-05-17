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

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables")
		bool Shooting = false;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Variables")
		float Range = 100.0f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Variables")
		FVector2D UpperSpread = FVector2D(0,0);

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Variables")
		FVector2D LowerSpread = FVector2D(0, 0);

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Variables")
		int BulletsPerShot = 1;
	
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Variables")
		float ShotsPerSecond = 1;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Variables")
		bool Automatic = false;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables")
		float ShootingTimer = 0;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables")
		float SecondsBetweenShots = 0;


public:

	FCollisionQueryParams ignoredActors;

public:	
	// Sets default values for this actor's properties
	AGunBase();

	void Shoot();
	void StopShoot();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Custom | Shoot")
		void ShootRaycasts();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

};
