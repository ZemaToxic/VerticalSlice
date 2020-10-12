// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"

#include "Rocket.generated.h"

UCLASS()
class VERTICALSLICE_API ARocket : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* RocketMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* RocketMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* RocketCollision;

	UPROPERTY(EditAnywhere, Category = "Custom | Launch Variables", meta = (AllowPrivateAccess = "true"))
		FVector LaunchLoc = FVector();

	UPROPERTY(EditAnywhere, Category = "Custom | Launch Variables", meta = (AllowPrivateAccess = "true"))
		FVector AimLoc = FVector();

	bool AtLaunch = false;

	UPROPERTY(EditAnywhere, Category = "Custom | Launch Variables", meta = (AllowPrivateAccess = "true"))
		float SpeedForTurning = 100;

	UPROPERTY(EditAnywhere, Category = "Custom | Launch Variables", meta = (AllowPrivateAccess = "true"))
		float SpeedForLaunch = 600;

	UPROPERTY(EditAnywhere, Category = "Custom | Launch Variables", meta = (AllowPrivateAccess = "true"))
		float AccelerationAmount = 10;

	UPROPERTY(EditAnywhere, Category = "Custom | Launch Variables", meta = (AllowPrivateAccess = "true"))
		float RotationInterp = 0.05;

	UPROPERTY(EditAnywhere, Category = "Custom | Launch Variables", meta = (AllowPrivateAccess = "true"))
		float RotationTolerance = 0.01;

	UPROPERTY(EditAnywhere, Category = "Custom | Damage", meta = (AllowPrivateAccess = "true"))
		float ExplosionRadius = 500;

	UPROPERTY(EditAnywhere, Category = "Custom | Damage", meta = (AllowPrivateAccess = "true"))
		float Damage = 0;

	UPROPERTY(EditAnywhere, Category = "Custom | Launch Variables", meta = (AllowPrivateAccess = "true"))
		float MaxFlyTime = 10;

	UPROPERTY(VisibleAnywhere, Category = "Custom | Launch Variables", meta = (AllowPrivateAccess = "true"))
		FTimerHandle FlyTime;

	UPROPERTY(EditAnywhere, Category = "Custom | VFX", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* ExplosionFX;

	bool PastLaunchLocation = false;

public:	
	// Sets default values for this actor's properties
	ARocket();
	void Init(FVector _LaunchLoc, FVector _AimLoc, float _Damage, float _ExplosionRadius);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Explode")
		void Explode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
