// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "GunBase.generated.h"


UCLASS()
class VERTICALSLICE_API AGunBase : public AActor
{
	GENERATED_BODY()
		
	//components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "CustomVariables | Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "CustomVariables | Muzzle", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Muzzle;

	//gun behaviour variables
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		float MaxRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		float OptimalRangePercent = 0.5f;

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

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		float Damage = 30;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		float DamageRange = 10;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		float DamageFalloff = 20;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		float FalloffCurve = 0.01;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Behaviour")
		bool DestroysArmourPlate = false;

	//gun internal properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Watchables", meta = (AllowPrivateAccess = "true"))
		bool Shooting = false;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables")
		float ShootingTimer = 0;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables")
		float SecondsBetweenShots = 0;


	//gun ammo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Ammo", meta = (AllowPrivateAccess = "true"))
		int CurrentMagsize = 30;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Ammo")
		int MaxMagsize = 30;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Ammo")
		bool usesBullets = true;

	//mech
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Watchables", meta = (AllowPrivateAccess = "true"))
		class AMech* AttachedMech = 0;

	//effects
	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Effects")
		UAnimMontage* shootingAnimation = 0;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Effects")
		UNiagaraSystem* HitPS = 0;

	//variables for particle effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | PE Variables", meta = (AllowPrivateAccess = "true"))
		FVector shotStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | PE Variables", meta = (AllowPrivateAccess = "true"))
		TArray<FVector> shotEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | PE Variables", meta = (AllowPrivateAccess = "true"))
		TArray<FHitResult> hitResults;
public:

	FCollisionQueryParams ignoredActors;

public:	
	// Sets default values for this actor's properties
	AGunBase();

	void init(class AMech* mech);

	void Shoot();
	void StopShoot();

	void setShootAnim(class UAnimMontage* newAnim);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Custom | Shoot")
		void ShootRaycasts();

	void Reload(int& ammoPool);
	bool hasMaxMag() { return CurrentMagsize == MaxMagsize; }

	void UpgradeDamage(float _Multiplier);
	void UpgradeClipSize(float _Multiplier);
	void UpgradeBulletsPerShot(float _Multiplier);
	void UpgradeRange(float _Multiplier);

protected:
	virtual void BeginPlay() override;

	float CalcDamage(float Dist);

public:
	virtual void Tick(float DeltaTime) override;
};
