// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mech.generated.h"

UCLASS()
class VERTICALSLICE_API AMech : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Camera")
		float BoomBaseTarget = 300;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Camera")
		float BoomAimTarget = 200;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Camera")
		float BoomTargetInterpRate = 10;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Camera")
		float BoomCurrentTarget = 300;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Camera")
		bool BoomCurrentTargetChange = true;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Camera")
		float CameraBaseFOV = 90;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Camera")
		float CameraAimFOV = 60;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Camera")
		float CameraSprintFOV = 120;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Camera")
		float CameraFOVInterpRate = 10;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Camera")
		float CameraCurrentFOV = 90;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Camera")
		bool CameraCurrentFOVChange = true;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Watchables")
		bool Aiming = false;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Movement")
		float BaseWalkSpeed = 600;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Movement")
		float AimWalkSpeed = 300;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Movement")
		float SprintWalkSpeed = 1200;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Watchables")
		bool Sprinting = false;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee")
		float MeleeDamage = 10;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee")
		 FVector MeleeRange = FVector(30,30,70);

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Gun")
		TSubclassOf<class AGunBase> GunClass;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Gun")
		class AGunBase* Gun = 0;

public:
	// Sets default values for this character's properties
	AMech();

protected:

	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Aim();
	void StopAim();

	void Sprint();
	void StopSprint();

	void Melee();

	void Shoot();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
