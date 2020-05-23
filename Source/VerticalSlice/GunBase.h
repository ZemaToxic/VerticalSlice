// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

UENUM()
enum GunUpgrades
{
	BetterFireRate,
	FasterReload,
	BetterDamage,
};


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

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		float Range = 100.0f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		FVector2D UpperSpread = FVector2D(0,0);

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		FVector2D LowerSpread = FVector2D(0, 0);

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		int BulletsPerShot = 1;
	
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		float ShotsPerSecond = 1;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		bool Automatic = false;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables")
		float ShootingTimer = 0;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables")
		float SecondsBetweenShots = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Ammo", meta = (AllowPrivateAccess = "true"))
		int CurrentMagsize = 30;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Ammo")
		int MaxMagsize = 30;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		float Damage = 30;

public:

	FCollisionQueryParams ignoredActors;

public:	
	// Sets default values for this actor's properties
	AGunBase();

	void Shoot();
	void StopShoot();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Custom | Shoot")
		void ShootRaycasts();

	void Reload(int& ammoPool);

	UFUNCTION(BlueprintCallable, Category = "Custom | Upgrade")
		void Upgrade(GunUpgrades upgrade);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
