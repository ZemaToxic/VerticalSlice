// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech.h"
#include "MonsterBase.h"
#include "VerticalSliceCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"
#include "NiagaraFunctionLibrary.h"

#include "DrawDebugHelpers.h"
#include "Engine.h"


AMech::AMech()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}


void AMech::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("This")));
	if (GunClass)
	{
		Gun = GetWorld()->SpawnActor<AGunBase>(GunClass);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GunSocket"));
		Gun->init(this, GunClass);
		Gun->setShootAnim(HipShoot);
	}

	if (ShotgunClass)
	{
		Shotgun = GetWorld()->SpawnActor<AGunBase>(ShotgunClass);
		Shotgun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ShotgunSocket"));
		Shotgun->init(this, ShotgunClass);
		Shotgun->setShootAnim(ShotgunShoot);
	}

	if (FlamethrowerClass)
	{
		Flamethrower = GetWorld()->SpawnActor<AFlamethrowerBase>(FlamethrowerClass);
		Flamethrower->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ShotgunSocket"));
		Flamethrower->init(this, FlamethrowerClass);
		Flamethrower->setShootAnim(FlamethrowerShoot);
		Shotgun->ignoredActors.AddIgnoredActor(Flamethrower);
		Flamethrower->ignoredActors.AddIgnoredActor(Shotgun);
	}

	if (RocketLauncherClass)
	{
		RocketLauncher = GetWorld()->SpawnActor<ARocketLauncher>(RocketLauncherClass);
		RocketLauncher->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RocketSocket"));
		RocketLauncher->init(this, RocketLauncherClass);
		RocketLauncher->setShootAnim(RocketLauncherShoot);
	}

	BoomCurrentTarget = BoomBaseTarget;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GunSnapping = true;
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	GetCharacterMovement()->GravityScale = BaseGravityScale;

	ReloadPoint = ReloadPoint / ReloadSpeed;

	DefaultMech = MechBPClass.GetDefaultObject();

	/*FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PlayerChar = GetWorld()->SpawnActor<AVerticalSliceCharacter>(PlayerClass, FVector(0,0,0), GetActorRotation(), spawnParams);
	PlayerChar->initalise(this);*/
}

// Called to bind functionality to input
void AMech::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMech::JumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMech::JumpEnd);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMech::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMech::StopAim);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMech::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMech::StopSprint);

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AMech::Melee);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMech::Shoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMech::StopShoot);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMech::Reload);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMech::Dash);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMech::Dismount);

	PlayerInputComponent->BindAction("Shotgun", IE_Pressed, this, &AMech::UseAbility);
	PlayerInputComponent->BindAction("Shotgun", IE_Released, this, &AMech::StopAbility);

	PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &AMech::SwitchAbilityInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMech::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMech::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMech::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMech::LookUp);
}

void AMech::Landed(const FHitResult& Hit)
{

	if (IsGroundPounding)
	{
		DoGroundPound();
	}
	else 
	{
		if (LandingCS)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(LandingCS);
		}
	}
	if (LandFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LandFX, GetActorLocation() + (GetActorUpVector() * -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	}
}

bool AMech::CanGroundPound()
{
	if (!GetCharacterMovement()->IsFalling() || !FeatureUpgradesMap[FeatureUpgrades::GroundPound] || CurrentCharge - GroundPoundChargeCost < 0) { return false; }

	giveCharge(false, -GroundPoundChargeCost);
	IsGroundPounding = true;
	GetCharacterMovement()->GravityScale = GroundPoundGravityScale;
	GetCharacterMovement()->StopMovementImmediately();
	AirControlTemp = GetCharacterMovement()->AirControl;
	GetCharacterMovement()->AirControl = 0;
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	return true;
}

void AMech::DoGroundPound_Implementation()
{
	if (GroundPoundCS)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(GroundPoundCS);
	}

	if (GroundPoundFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GroundPoundFX, GetActorLocation() + (GetActorUpVector() * -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	}

	// create tarray for hit results
	TArray<FHitResult> OutHits;

	// start and end locations
	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = SweepStart + FVector(0.1, 0, 0);

	// create a collision sphere
	FCollisionShape MyGroundPoundColl = FCollisionShape::MakeSphere(GroundPoundBaseRange + (GetCharacterMovement()->Velocity.Size() * GroundPoundRangeScale));

	// draw collision box
	//DrawDebugBox(GetWorld(), SweepStart, MyMeleeColl.GetExtent(), FColor::Purple, false, 1.0f);

	// check if something got hit in the sweep
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel1, MyGroundPoundColl);

	if (isHit)
	{
		TArray<AMonsterBase*> HitMonsters;
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			AMonsterBase* HitActor = Cast<AMonsterBase>(Hit.GetActor());
			if (HitActor)
			{
				if (!(HitMonsters.Contains(HitActor)))
				{
					HitMonsters.Add(HitActor);

					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *(Hit.Actor.Get()->GetName())));

					FVector launchDirection = HitActor->GetActorLocation() - this->GetActorLocation();
					launchDirection.Z = GroundPoundLaunchZ;

					float dist = launchDirection.Size();
					launchDirection.Normalize();

					HitActor->DamageMonster(GroundPoundDamage, HitActor->GetActorLocation(), Hit.BoneName, 0);
					HitActor->StunMonster(GroundPoundStunTime, launchDirection * GroundPoundLaunchPower);
				}
			}
		}
	}

	IsGroundPounding = false;
	GetCharacterMovement()->GravityScale = BaseGravityScale;
	GetCharacterMovement()->AirControl = AirControlTemp;
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void AMech::initalise(AVerticalSliceCharacter* Player)
{
	PlayerChar = Player;
}

void AMech::MoveForward(float Value)
{
	MoveForwardAxis = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMech::MoveRight(float Value)
{
	MoveRightAxis = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMech::LookUp(float Value)
{
	AddControllerPitchInput(Value * LookSensitivity);
}

void AMech::Turn(float Value)
{
	AddControllerYawInput(Value * LookSensitivity);
}

void AMech::Aim_Implementation()
{
	if (Sprinting)
	{
		StopSprint();
	}
	BoomCurrentTarget = BoomAimTarget;
	BoomCurrentTargetChange = true;
	CameraCurrentFOV = CameraAimFOV;
	CameraCurrentFOVChange = true;
	Aiming = true;
	//bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;

	Gun->setShootAnim(AimShoot);

	//GunSnapping = true;
}

void AMech::StopAim_Implementation()
{
	if (!Sprinting)
	{
		Gun->setShootAnim(HipShoot);
		BoomCurrentTarget = BoomBaseTarget;
		BoomCurrentTargetChange = true;
		CameraCurrentFOV = CameraBaseFOV;
		CameraCurrentFOVChange = true;
		Aiming = false;
		//bUseControllerRotationYaw = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		//GunSnapping = false;
	}
}

void AMech::Damage_Implementation(float dmg, FVector Loc)
{
	if (DamageFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamageFX, Loc);
	}
	giveHealth(false, -dmg);
}

void AMech::ChangeInput(bool Enable)
{
	APlayerController* PController = Cast<APlayerController>(GetController());
	if (PController)
	{
		if (Enable)
		{
			EnableInput(PController);
		}
		else
		{
			DisableInput(PController);
		}
	}
}

void AMech::JumpStart()
{
	if (CurrentCharge - JumpChargeCost < 0) { return; }
	//GetMovementComponent()->StopMovementImmediately();
	JumpInput = true;
}

void AMech::JumpEnd()
{
	if (GetCharacterMovement()->IsFalling())
	{
		StopJumping();
	}
	JumpInput = false;
}

void AMech::JumpTakeOff()
{
	Jump();
	giveCharge(false, -JumpChargeCost);
}

void AMech::JumpLand()
{
	if (bPressedJump && JumpInput)
	{
		StopJumping();
	}
}

void AMech::Mount_Implementation()
{
}

void AMech::Dash()
{
	if (FeatureUpgradesMap[FeatureUpgrades::Dash])
	{
		if (CurrentCharge - DashChargeCost > 0 && !HasDashed && (MoveRightAxis != 0 || MoveForwardAxis != 0))
		{
			HasDashed = true;
			//FVector launchDir = FVector(FVector2D(GetVelocity()), 0);
			FVector launchDir = (GetActorRightVector() * MoveRightAxis) + (GetActorForwardVector() * MoveForwardAxis);
			launchDir.Normalize();
			giveCharge(false, -DashChargeCost);
			LaunchCharacter(launchDir * DashForce, false, false);
		}
	}
}

void AMech::UpgradeFeatures(FeatureUpgrades _Upgrade, bool _Enable = true)
{
	FeatureUpgradesMap[_Upgrade] = _Enable;

	switch (_Upgrade)
	{
	case FeatureUpgrades::Boosters:
		break;
	case FeatureUpgrades::Shotgun:
		ActiveAbility = 0;
		break;
	case FeatureUpgrades::Dash:
		break;
	case FeatureUpgrades::GroundPound:
		break;
	case FeatureUpgrades::NoChargeRegenDelay:
		break;
	case FeatureUpgrades::HPRegen:
		break;
	case FeatureUpgrades::HPPotion://to do
		break;
	case FeatureUpgrades::Flamethrower:
		ActiveAbility = 1;
		break;
	case FeatureUpgrades::RocketLauncher:
		ActiveAbility = 2;
		break;
	default:
		break;
	}
}

void AMech::UpgradeStats(StatUpgrades _Upgrade, int _Amount, bool _Add)
{
	StatUpgradesMap[_Upgrade] = (_Add)? StatUpgradesMap[_Upgrade] + _Amount :_Amount;
	switch (_Upgrade)
	{
	case StatUpgrades::RifleDamage:
		Gun->UpgradeDamage(StatUpgradesMap[_Upgrade]);
		break;

	case StatUpgrades::RifleReload:
		ReloadSpeed = DefaultMech->ReloadSpeed + ReloadSpeedIncrement * StatUpgradesMap[_Upgrade];
		ReloadPoint = DefaultMech->ReloadPoint / ReloadSpeed;
		break;

	case StatUpgrades::RifleClipSize:
		Gun->UpgradeClipSize(StatUpgradesMap[_Upgrade]);
		break;

	case StatUpgrades::RifleReserveAmmo:
		MaxAmmo =  DefaultMech->MaxAmmo + AmmoIncrement * StatUpgradesMap[_Upgrade];
		CurrentAmmo = MaxAmmo;
		break;

	case StatUpgrades::ShotgunDamage:
		Shotgun->UpgradeDamage(StatUpgradesMap[_Upgrade]);
		break;

	case StatUpgrades::ShotgunCharges:
		Shotgun->UpgradeClipSize(StatUpgradesMap[_Upgrade]);
		break;

	case StatUpgrades::ShotgunPellets:
		Shotgun->UpgradeBulletsPerShot(StatUpgradesMap[_Upgrade]);
		break;

	case StatUpgrades::ShotgunRange:
		Shotgun->UpgradeRange(StatUpgradesMap[_Upgrade]);
		break;

	case StatUpgrades::MechMaxHP:
		MaxHealth =  DefaultMech->MaxHealth + HealthIncrement * StatUpgradesMap[_Upgrade];
		CurrentHealth = MaxHealth;
		break;

	case StatUpgrades::MechMaxCharge:
		MaxCharge = DefaultMech->MaxCharge + ChargeIncrement * StatUpgradesMap[_Upgrade];
		CurrentCharge = MaxCharge;
		break;

	case StatUpgrades::MechHPRegen:
		HealthRechargeRatePerSecond = DefaultMech->HealthRechargeRatePerSecond + HealthRegenIncrement * StatUpgradesMap[_Upgrade];
		break;

	case StatUpgrades::MechChargeRegen:
		ChargeRechargeRatePerSecond = DefaultMech->ChargeRechargeRatePerSecond + ChargeRegenIncrement * StatUpgradesMap[_Upgrade];
		break;

	case StatUpgrades::FlamethrowerDamage:
		Flamethrower->UpgradeDamage(StatUpgradesMap[_Upgrade]);
		break;

	case StatUpgrades::FlamethrowerFireDamage:
		Flamethrower->UpgradeFireDamage(StatUpgradesMap[_Upgrade]);
		break;

	case StatUpgrades::RocketAmount:
		RocketLauncher->UpgradeClipSize(StatUpgradesMap[_Upgrade]);
		break;

	case StatUpgrades::RocketRadius:
		RocketLauncher->UpgradeExplosionRadius(StatUpgradesMap[_Upgrade]);
		break;

	default:
		break;

	}
}

void AMech::MasterUpgrade(TMap<FeatureUpgrades,bool> _FeatureUpgradesMap, TMap<StatUpgrades,int> _StatUpgradesMap, bool &_Completed)
{
	_Completed = (Gun && Shotgun);
	if (!_Completed) { return; }

	for (auto& Feature : _FeatureUpgradesMap)
	{
		UpgradeFeatures(Feature.Key, Feature.Value);
	}

	for (auto& Stat : _StatUpgradesMap)
	{
		UpgradeStats(Stat.Key, Stat.Value, false);
	}
}

void AMech::UpgradeStatsUsingCurrent()
{
	for (auto& Stat : StatUpgradesMap)
	{
		UpgradeStats(Stat.Key, Stat.Value, false);
	}
}

void AMech::Sprint()
{
	if (FeatureUpgradesMap[FeatureUpgrades::Boosters])
	{
		if (Aiming)
		{
			StopAim();
		}
		GetCharacterMovement()->MaxWalkSpeed = SprintWalkSpeed;
		Sprinting = true;
	}
}

void AMech::StopSprint()
{
	if (!Aiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		CameraCurrentFOV = CameraBaseFOV;
		CameraCurrentFOVChange = true;
		Sprinting = false;
	}
}

void AMech::Melee()
{
	if (CanGroundPound()) { return; }
	if (MeleeAnim)
	{
		UAnimInstance* mechAnim = GetMesh()->GetAnimInstance();
		if (!mechAnim->Montage_IsPlaying(MeleeAnim))
		{
			mechAnim->Montage_Play(MeleeAnim);
		}
		else
		{
			return;
		}
	}
	
	//// create tarray for hit results
	//TArray<FHitResult> OutHits;

	//FVector MeleeDir = GetActorForwardVector();

	//// start and end locations
	//FVector SweepStart = GetActorLocation() + (MeleeDir*(MeleeRange.Y));
	//FVector SweepEnd = SweepStart + (MeleeDir*0.1);

	//// create a collision sphere
	//FCollisionShape MyMeleeColl = FCollisionShape::MakeBox(MeleeRange);	
	//
	//// draw collision box
	////DrawDebugBox(GetWorld(), SweepStart, MyMeleeColl.GetExtent(), FColor::Purple, false, 1.0f);

	//// check if something got hit in the sweep
	//bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_Visibility, MyMeleeColl, Gun->ignoredActors);

	//if (isHit)
	//{
	//	TArray<AMonsterBase*> HitMonsters;
	//	// loop through TArray
	//	for (auto& Hit : OutHits)
	//	{
	//		AMonsterBase* HitActor = Cast<AMonsterBase>(Hit.GetActor());
	//		if (HitActor)
	//		{ 
	//			if (!(HitMonsters.Contains(HitActor)))
	//			{
	//				HitMonsters.Add(HitActor);
	//				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), MeleeKnockback));
	//				HitActor->DamageMonster(MeleeDamage, HitActor->GetActorLocation(), Hit.BoneName, MeleeKnockback);
	//			}
	//		}
	//	}
	//}
}

void AMech::Shoot()
{
	if (Gun)
	{
		Gun->Shoot();
	}
}

void AMech::StopShoot()
{
	if (Gun)
	{
		Gun->StopShoot();
	}
}

void AMech::Reload()
{
	if (ReloadAnim && !(Gun->hasMaxMag()) && CurrentAmmo > 0)
	{
		UAnimInstance* mechAnim = GetMesh()->GetAnimInstance();
		if (!(mechAnim->Montage_IsPlaying(ReloadAnim)))
		{
			mechAnim->Montage_Play(ReloadAnim, ReloadSpeed);
			reloading = true;
		}
	}
}

FVector AMech::GetCameraLookLocation(float _Range, float &_Dist)
{
	FHitResult Hit;
	FVector TraceStart = FollowCamera->GetComponentLocation();
	FVector VDist = (FollowCamera->GetForwardVector() * (CameraBoom->TargetArmLength + _Range));
	_Dist = VDist.Size();;
	FVector TraceEnd = TraceStart + VDist;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Camera, Gun->ignoredActors);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s, %s"), *(Hit.Actor.Get()->GetName()) ,*(Hit.GetComponent()->GetName())));

	if (Hit.bBlockingHit)
	{
		_Dist = Hit.Distance;
		return Hit.Location;
	}
	return Hit.TraceEnd;
}

void AMech::Dismount_Implementation()
{
	if (PlayerChar && !(GetMovementComponent()->IsFalling()))
	{
		FVector spawnLoc = GetActorLocation() + GetActorForwardVector() * 500;

		FHitResult SweepHit;


		GetWorld()->SweepSingleByChannel(SweepHit, GetActorLocation(), spawnLoc, FQuat::Identity, ECollisionChannel::ECC_Pawn, PlayerChar->GetCapsuleComponent()->GetCollisionShape(), Gun->ignoredActors);
		if (SweepHit.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, SweepHit.Actor.Get()->GetName());
			return;
		}

		PlayerChar->SetActorLocationAndRotation(spawnLoc, GetActorRotation());

		PlayerChar->SetVisible(true, true, true);
		AController* controller = GetController();
		//AController* controller2 = PlayerChar->GetController();
		controller->UnPossess();
		//controller2->UnPossess();
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		controller->Possess(Cast<APawn>(PlayerChar));
		//controller2->Possess(Cast<APawn>(this));
		SpawnDefaultController();
		StopAim();
		StopSprint();
		canMount = false;
	}
}

void AMech::UseAbility()
{
	if (FeatureUpgradesMap[FeatureUpgrades::Shotgun] && ActiveAbility == 0)
	{
		Shotgun->Shoot();
		if (!GetWorldTimerManager().IsTimerActive(ShotgunTimerHandle))
		{
			GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AMech::ShotgunRecharge, ShotgunCooldown);
		}
	}
	else if (FeatureUpgradesMap[FeatureUpgrades::Flamethrower] && ActiveAbility == 1)
	{
		GetWorldTimerManager().ClearTimer(FlamethrowerTimerHandle);
		Flamethrower->Shoot();
		FlamethrowerCanRecharge = false;
	}
	else if (FeatureUpgradesMap[FeatureUpgrades::RocketLauncher] && ActiveAbility == 2)
	{
		RocketLauncher->Shoot();
		if (!GetWorldTimerManager().IsTimerActive(RocketLauncherTimerHandle))
		{
			GetWorldTimerManager().SetTimer(RocketLauncherTimerHandle, this, &AMech::RocketLauncherRecharge, RocketLauncherCooldown);
		}
	}
	else if (SwitchAbility())
	{
		UseAbility();
	}
}

void AMech::StopAbility()
{
	Flamethrower->StopShoot();
	GetWorldTimerManager().SetTimer(FlamethrowerTimerHandle, this, &AMech::FlamethrowerRecharge, FlamethrowerRechargeDelay);

	Shotgun->StopShoot();

	RocketLauncher->StopShoot();
}

void AMech::ShotgunRecharge()
{
	if (!Shotgun->Reload(1))
	{
		GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AMech::ShotgunRecharge, ShotgunCooldown);
	}
}

void AMech::FlamethrowerRecharge()
{
	FlamethrowerCanRecharge = true;
}

void AMech::RocketLauncherRecharge()
{
	if (!RocketLauncher->Reload(1))
	{
		GetWorldTimerManager().SetTimer(RocketLauncherTimerHandle, this, &AMech::RocketLauncherRecharge, RocketLauncherCooldown);
	}
}

bool AMech::SwitchAbility()
{
	bool Available = false;
	while (!Available)
	{
		ActiveAbility++;
		ActiveAbility %= 3;
		if (ActiveAbility == 0 && FeatureUpgradesMap[FeatureUpgrades::Shotgun])
		{
			Available = true;
		}
		else if (ActiveAbility == 1 && FeatureUpgradesMap[FeatureUpgrades::Flamethrower])
		{
			Available = true;
		}
		else if (ActiveAbility == 2 && FeatureUpgradesMap[FeatureUpgrades::RocketLauncher])
		{
			Available = true;
		}
		else if (!FeatureUpgradesMap[FeatureUpgrades::Shotgun] && !FeatureUpgradesMap[FeatureUpgrades::Flamethrower] && !FeatureUpgradesMap[FeatureUpgrades::RocketLauncher])
		{
			ActiveAbility = -1;
			break;
		}
	}
	return Available;
}

void AMech::giveAmmo(bool Max, int amount)
{
	if (Max || CurrentAmmo + amount >= MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
		int TempAmmo = 10000;
		Gun->Reload(TempAmmo);
		Shotgun->Reload(TempAmmo);
		Flamethrower->Reload(TempAmmo);
		RocketLauncher->Reload(TempAmmo);
	}
	else if(CurrentAmmo + amount > 0)
	{
		CurrentAmmo += amount;
	}
	else
	{
		CurrentAmmo = 0;
	}
}

void AMech::giveHealth(bool Max, int amount)
{
	if (Max || CurrentHealth + amount >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	else
	{
		if (CurrentHealth + amount > 0)
		{
			CurrentHealth += amount;
		}
		else
		{
			CurrentHealth = 0;
		}

		HealthRechargeAllowed = false;

		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([=]
			{
				HealthRechargeAllowed = true;
			});

		GetWorldTimerManager().SetTimer(HealthRechargeTimer, TimerCallback, HealthRechargeDelay, false);
	}
}

void AMech::giveCharge_Implementation(bool Max, int amount)
{
	if (Max || CurrentCharge + amount >= MaxCharge)
	{
		CurrentCharge = MaxCharge;
	}
	else
	{
		if (CurrentCharge + amount > 0)
		{
			CurrentCharge += amount;
		}
		else
		{
			CurrentCharge = 0;
		}
		if (!FeatureUpgradesMap[FeatureUpgrades::NoChargeRegenDelay])
		{
			ChargeRechargeAllowed = false;

			FTimerDelegate TimerCallback;
			TimerCallback.BindLambda([=]
				{
					ChargeRechargeAllowed = true;
				});

			GetWorldTimerManager().SetTimer(ChargeRechargeTimer, TimerCallback, ChargeRechargeDelay, false);
		}
	}
}

// Called every frame
void AMech::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlamethrowerRechargeTimer += DeltaTime;

	if (BoomCurrentTargetChange)
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, BoomCurrentTarget, DeltaTime, BoomTargetInterpRate);
		BoomCurrentTargetChange = CameraBoom->TargetArmLength != BoomCurrentTarget;
	}

	if (CameraCurrentFOVChange)
	{
		FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, CameraCurrentFOV, DeltaTime, CameraFOVInterpRate);
		CameraCurrentFOVChange = FollowCamera->FieldOfView != CameraCurrentFOV;
	}

	if (Sprinting && !GetCharacterMovement()->Velocity.IsZero() && CameraCurrentFOV != CameraSprintFOV)
	{
		CameraCurrentFOV = CameraSprintFOV;
		CameraCurrentFOVChange = true;
	}
	else if (Sprinting && GetCharacterMovement()->Velocity.IsZero() && CameraCurrentFOV != CameraBaseFOV)
	{
		CameraCurrentFOV = CameraBaseFOV;
		CameraCurrentFOVChange = true;
	}

	if (ReloadAnim && reloading)
	{
		UAnimInstance* mechAnim = GetMesh()->GetAnimInstance();
		if (mechAnim->Montage_IsPlaying(ReloadAnim))
		{
			if (mechAnim->Montage_GetPosition(ReloadAnim) > ReloadPoint)
			{
				if (Gun)
				{
					Gun->ReloadUsingAmmoPool(CurrentAmmo);
				}
				reloading = false;
			}
		}
		else
		{
			reloading = false;
		}
	}

	if (Sprinting && !(GetCharacterMovement()->Velocity.IsZero()))
	{
		if (CurrentCharge > 0)
		{
			giveCharge(false, -SprintingChargeCostPerSecond * DeltaTime);
		}
		else
		{
			StopSprint();
		}
	}
	else if (ChargeRechargeAllowed && CurrentCharge < MaxCharge)
	{
		if (CurrentCharge + ChargeRechargeRatePerSecond * DeltaTime < MaxCharge)
		{
			CurrentCharge += ChargeRechargeRatePerSecond * DeltaTime;
		}
		else
		{
			CurrentCharge = MaxCharge;
		}
	}

	if (HealthRechargeAllowed && CurrentHealth < MaxHealth && FeatureUpgradesMap[FeatureUpgrades::HPRegen])
	{
		if (CurrentHealth + HealthRechargeRatePerSecond * DeltaTime < MaxHealth)
		{
			CurrentHealth += HealthRechargeRatePerSecond * DeltaTime;
		}
		else
		{
			CurrentHealth = MaxHealth;
		}
	}

	if (FlamethrowerRechargeTimer > 1.0f / FlamethrowerAmmoPerSecond)
	{
		if (FlamethrowerCanRecharge)
		{
			Flamethrower->Reload(1);
		}
		FlamethrowerRechargeTimer = 0.0f;
	}
}