// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech.h"
#include "GunBase.h"
#include "MonsterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"

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
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
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
	}

	BoomCurrentTarget = BoomBaseTarget;
}

// Called to bind functionality to input
void AMech::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMech::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMech::StopAim);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMech::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMech::StopSprint);

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AMech::Melee);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMech::Shoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMech::StopShoot);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMech::Reload);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMech::Dash);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMech::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMech::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AMech::MoveForward(float Value)
{
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
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;

	GunSnapping = true;
}

void AMech::StopAim_Implementation()
{
	if (!Sprinting)
	{
		BoomCurrentTarget = BoomBaseTarget;
		BoomCurrentTargetChange = true;
		CameraCurrentFOV = CameraBaseFOV;
		CameraCurrentFOVChange = true;
		Aiming = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		GunSnapping = false;
	}
}

void AMech::Damage(float dmg)
{
	if (CurrentHealth - dmg >= 0)
	{
		CurrentHealth -= dmg;
	}
	else
	{
		CurrentHealth = 0;
	}
}

void AMech::Dash()
{
	if (CurrentStamina - DashStamina > 0)
	{
		LaunchCharacter(GetActorForwardVector() * DashForce, false, false);
	}
}

void AMech::Upgrade(MechUpgrades upgrade)
{
	switch (upgrade)
	{
	case StaminaRegen:
		break;
	case MoreAmmo:
		break;
	default:
		break;
	}
}

void AMech::UpgradeAbilities(AbilityUpgrades upgrade)
{
	switch (upgrade)
	{
	case ShorterCooldown:
		break;
	case ExtraCharge:
		break;
	case Dragonbreath:
		break;
	default:
		break;
	}
}

void AMech::Sprint()
{
	if (Aiming)
	{
		StopAim();
	}
	GetCharacterMovement()->MaxWalkSpeed = SprintWalkSpeed;
	Sprinting = true;
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
	// create tarray for hit results
	TArray<FHitResult> OutHits;

	FVector MeleeDir = GetActorForwardVector();

	// start and end locations
	FVector SweepStart = GetActorLocation() + (MeleeDir*(MeleeRange.Y));
	FVector SweepEnd = SweepStart + (MeleeDir*0.1);

	// create a collision sphere
	FCollisionShape MyMeleeColl = FCollisionShape::MakeBox(MeleeRange);	
	
	// draw collision box
	DrawDebugBox(GetWorld(), SweepStart, MyMeleeColl.GetExtent(), FColor::Purple, false, 1.0f);

	// check if something got hit in the sweep
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_Visibility, MyMeleeColl, Gun->ignoredActors);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			AMonsterBase* HitActor = Cast<AMonsterBase>(Hit.GetActor());
			if (HitActor)
			{
				HitActor->DamageMonster(MeleeDamage, Hit.Location, Hit.BoneName);
			}
		}
	}
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
	if (Gun)
	{
		Gun->Reload(CurrentAmmo);
	}
}

// Called every frame
void AMech::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	FRotator GunRotation = Gun->GetActorRotation();

	if (GunSnapping)
	{
		FRotator CameraRotation = FollowCamera->GetComponentRotation();
		Gun->SetActorRotation(FRotator(CameraRotation.Pitch, GunRotation.Yaw, GunRotation.Roll));
	}
	else if (Gun->GetActorRotation() != FRotator(0, GunRotation.Yaw, GunRotation.Roll))
	{
		Gun->SetActorRotation(FRotator(0, GunRotation.Yaw, GunRotation.Roll));
	}

	if (Sprinting)
	{
		if (CurrentStamina > 0)
		{
			CurrentStamina--;
		}
		else
		{
			StopSprint();
		}
	}
	else if (CurrentStamina < MaxStamina)
	{
		if (CurrentStamina + StaminaRechargeRate < MaxStamina)
		{
			CurrentStamina += StaminaRechargeRate;
		}
		else
		{
			CurrentStamina = MaxStamina;
		}
	}
}

