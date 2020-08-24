// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GunBase.h"
#include "Mech.generated.h"

UENUM()
enum class FeatureUpgrades : uint8
{
	Boosters,
	Shotgun,
	Dash,
	GroundPound,
	NoChargeRegenDelay,
	HPRegen,
	HPPotion,
	Flamethrower,
	RocketLauncher,
};

UENUM()
enum class StatUpgrades : uint8
{
	RifleDamage,
	RifleReload,
	RifleClipSize,
	RifleReserveAmmo,
	ShotgunDamage,
	ShotgunCharges,
	ShotgunPellets,
	ShotgunRange,
	MechMaxHP,
	MechMaxCharge,
	MechHPRegen,
	MechChargeRegen,
	FlamethrowerDamage,
	FlamethrowerFireDamage,
	RocketAmount,
	RocketRadius,
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
	///Camera variables
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


	///Movement variables
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Movement")
		float BaseWalkSpeed = 600;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Movement")
		float AimWalkSpeed = 300;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Movement")
		float SprintWalkSpeed = 1200;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Watchables")
		bool Sprinting = false;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Movement")
		float SprintingChargeCostPerSecond = 100;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Movement")
		float JumpChargeCost = 100;

	///Melee variables
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee")
		float MeleeDamage = 10;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee")
		 FVector MeleeRange = FVector(150,150,450);

	///Gun variables
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Gun")
		TSubclassOf<class AGunBase> GunClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Gun", meta = (AllowPrivateAccess = "true"))
		class AGunBase* Gun = 0;

	///Shotgun variables
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Gun")
		TSubclassOf<class AGunBase> ShotgunClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Gun", meta = (AllowPrivateAccess = "true"))
		class AGunBase* Shotgun = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Watchables | Ability", meta = (AllowPrivateAccess = "true"))
		FTimerHandle ShotgunTimerHandle;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Ability")
		float ShotgunCooldown = 2.0f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Ability")
		int MaxShotgunShots = 2;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Ability")
		int CurrentShotgunShots = 2;

	///resource variables
	//ammo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		int CurrentAmmo = 100;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Resources")
		int MaxAmmo = 100;

	int DefaultAmmo = 100;

	//health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float CurrentHealth = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float MaxHealth = 300;

	int DefaultHealth = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float HealthWarningThreshold = 0.33f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Resources")
		float HealthRechargeRate = 0.0f;

	float DefaultHealthRecharge = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		bool HealthRechargeAllowed = true;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		FTimerHandle HealthRechargeTimer;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float HealthRechargeDelay = 1.0;

	//charge(stamina)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float CurrentCharge = 600;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float MaxCharge = 600;

	int DefaultCharge = 600;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Resources")
		float ChargeRechargeRate = 1.0f;

	float DefaultChargeRecharge = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		bool ChargeRechargeAllowed = true;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		FTimerHandle ChargeRechargeTimer;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Resources", meta = (AllowPrivateAccess = "true"))
		float ChargeRechargeDelay = 1.0;

	///Dash Variables
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Dash")
		float DashChargeCost = 300;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Dash")
		float DashForce = 10000;

	///player character
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Player")
		TSubclassOf<class AVerticalSliceCharacter> PlayerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Gameplay | Player", meta = (AllowPrivateAccess = "true"))
		class AVerticalSliceCharacter* PlayerChar = 0;

	///Upgrade variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Watchables | Upgrade", meta = (AllowPrivateAccess = "true"))
		TMap<FeatureUpgrades,bool> FeatureUpgradesMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Watchables | Upgrade", meta = (AllowPrivateAccess = "true"))
		TMap<StatUpgrades, int> StatUpgradesMap;

	///animations
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

	///Animation variables
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CustomVariables | Gameplay | Watchables", meta = (AllowPrivateAccess = "true"))
		float MoveRightAxis = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CustomVariables | Gameplay | Watchables", meta = (AllowPrivateAccess = "true"))
		float MoveForwardAxis = 0;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables | Bools")
		bool reloading = false;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Watchables | Animation")
		float currentReloadPoint = 4.0f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Watchables | Animation")
		float reloadPoint = 4.0f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Animation")
		float reloadAnimationRate = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Animation", meta = (AllowPrivateAccess = "true"))
		bool JumpInput = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Animation", meta = (AllowPrivateAccess = "true"))
		bool IsGroundPounding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomVariables | Animation", meta = (AllowPrivateAccess = "true"))
		bool GunSnapping = true;

	///Ground pound Variables
	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee", meta = (AllowPrivateAccess = "true"))
		float BaseGravityScale = 10;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee", meta = (AllowPrivateAccess = "true"))
		float GroundPoundGravityScale = 100;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee", meta = (AllowPrivateAccess = "true"))
		float GroundPoundLaunchZ = 1000;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee", meta = (AllowPrivateAccess = "true"))
		float GroundPoundRangeScale = 1;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee", meta = (AllowPrivateAccess = "true"))
		float GroundPoundBaseRange = 1000;

	UPROPERTY(EditAnywhere, Category = "CustomVariables | Gameplay | Melee", meta = (AllowPrivateAccess = "true"))
		float GroundPoundLaunchPower = 10000;

	UPROPERTY(VisibleAnywhere, Category = "CustomVariables | Gameplay | Melee", meta = (AllowPrivateAccess = "true"))
		float AirControlTemp = 0;



public:
	//variable/s the player needs to see and change
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CustomVariables | Mount")
			bool canMount = true;

public:
	// Sets default values for this character's properties
	AMech();

protected:

	virtual void BeginPlay() override;

	void SetDefaults();

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
	void StopAbility();
	void AbilityRecharge();

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

	virtual void Landed(const FHitResult& Hit) override;
	bool GroundPound();

public:
	void Reload();

	FVector GetCameraLookLocation(float _Range, float& _Dist);

	UFUNCTION(BlueprintCallable, Category = "Custom | Reset")
		void giveAmmo(bool Max, int amount = 0);

	UFUNCTION(BlueprintCallable, Category = "Custom | Reset")
		void giveHealth(bool Max, int amount = 0);

	UFUNCTION(BlueprintCallable, Category = "Custom | Reset")
		void giveCharge(bool Max, int amount = 0);

	UFUNCTION(BlueprintCallable, Category = "Custom | Upgrade")
		void UpgradeFeatures(FeatureUpgrades _Upgrade, bool _Enable);

	UFUNCTION(BlueprintCallable, Category = "Custom | Upgrade")
		void UpgradeStats(StatUpgrades _Upgrade, int _Amount);

	UFUNCTION(BlueprintCallable, Category = "Custom | Upgrade")
		void UpgradeStatsAdd(StatUpgrades _Upgrade, int _Amount);

	UFUNCTION(BlueprintCallable, Category = "Custom | Upgrade")
		void MasterUpgrade(TMap<FeatureUpgrades,bool> _FeatureUpgradesMap, TMap<StatUpgrades,int> _StatUpgradesMap);

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
