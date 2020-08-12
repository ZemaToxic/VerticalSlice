// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GunBase.h"
#include "Mech.generated.h"

UENUM()
enum class MechUpgrades : uint8
{
	None,
	StaminaRegen,
	MoreAmmo,
	FasterReload,
};

UENUM()
enum class AbilityUpgrades : uint8
{
	None,
	ShorterCooldown,
	ExtraCharge,
	Dragonbreath,
};



UCLASS()
class VERTICALSLICE_API AMech : public ACharacter
{
	GENERATED_BODY()

private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

private:

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Watchables", meta = (AllowPrivateAccess = "true"))
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
		 FVector MeleeRange = FVector(150,150,450);

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Gun")
		TSubclassOf<class AGunBase> GunClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Gun", meta = (AllowPrivateAccess = "true"))
		class AGunBase* Gun = 0;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Gun")
		TSubclassOf<class AGunBase> ShotgunClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Gun", meta = (AllowPrivateAccess = "true"))
		class AGunBase* Shotgun = 0;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Watchables")
		bool GunSnapping = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		int CurrentAmmo = 100;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Resources")
		int MaxAmmo = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float CurrentHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float HealthWarningThreshold = 0.33f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float CurrentStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float MaxStamina = 100;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Resources")
		float StaminaRechargeRate = 1;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Dash")
		float DashStamina = 20;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Dash")
		float DashForce = 10000;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CustomVariables | Gameplay | Watchables", meta = (AllowPrivateAccess = "true"))
		float MoveRightAxis = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CustomVariables | Gameplay | Watchables", meta = (AllowPrivateAccess = "true"))
		float MoveForwardAxis = 0;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Player")
		TSubclassOf<class AVerticalSliceCharacter> PlayerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Player", meta = (AllowPrivateAccess = "true"))
		class AVerticalSliceCharacter* PlayerChar = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Watchables | Upgrade", meta = (AllowPrivateAccess = "true"))
		MechUpgrades LastMechUpgrade = MechUpgrades::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Watchables | Upgrade", meta = (AllowPrivateAccess = "true"))
		AbilityUpgrades LastAbilityUpgrade = AbilityUpgrades::None;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AimShoot = 0;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HipShoot = 0;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ShotgunShoot = 0;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* MeleeAnim = 0;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ReloadAnim = 0;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables | Bools")
		bool reloading = false;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables | Animation")
		float currentReloadPoint = 4.0f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Watchables | Animation")
		float reloadPoint = 4.0f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Animation")
		float reloadAnimationRate = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables | AbilityCooldown")
		bool canUseAbility = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Watchables | AbilityCooldown", meta = (AllowPrivateAccess = "true"))
		FTimerHandle abilityTimerHandle;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | AbilityCooldown")
		float abilityCooldown = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Animation | Jump", meta = (AllowPrivateAccess = "true"))
		bool JumpInput = false;

public:
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CustomVariables | Mount")
			bool canMount = true;

public:
	// Sets default values for this character's properties
	AMech();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Custom | Init")
		void initalise(class AVerticalSliceCharacter* Player);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Custom | Aim")
		void Aim();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Custom | Aim")
		void StopAim();

	void Dash();

	void Sprint();
	void StopSprint();

	void Melee();

	void Shoot();
	void StopShoot();

	void UseAbility();

	UFUNCTION()
		void AbilityReset();

	void JumpStart();
	void JumpEnd();

	UFUNCTION(BlueprintCallable, Category = "Custom | Animation | Jump")
		void JumpTakeOff();

	UFUNCTION(BlueprintCallable, Category = "Custom | Animation | Jump")
		void JumpLand();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Custom | Mount")
		void Dismount();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void Reload();

	FVector GetCameraLookLocation(float _Range);

	UFUNCTION(BlueprintCallable, Category = "Custom | Reset")
		void giveAmmo(bool Max, int amount = 0);

	UFUNCTION(BlueprintCallable, Category = "Custom | Reset")
		void giveHealth(bool Max, int amount = 0);

	UFUNCTION(BlueprintCallable, Category = "Custom | Reset")
		void giveStamina(bool Max, int amount = 0);

	UFUNCTION(BlueprintCallable, Category = "Custom | Upgrade")
		void Upgrade(MechUpgrades upgrade);

	UFUNCTION(BlueprintCallable, Category = "Custom | Upgrade")
		void UpgradeAbilities(AbilityUpgrades upgrade);

	UFUNCTION(BlueprintCallable, Category = "Custom | Upgrade")
		void MasterUpgrade(MechUpgrades mechUpgrade, AbilityUpgrades abilityUpgrade, GunUpgrades gunUpgrade);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Custom | Health")
		void Damage(float dmg, FVector Loc);

	UFUNCTION(BlueprintCallable, Category = "Custom | Animation")
		void ChangeInput(bool Enable);

	UFUNCTION(BlueprintNativeEvent, Category = "Custom | Mount")
		void Mount();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
