// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "VerticalSliceCharacter.h"
#include "Mech.h"
#include "interactableVolume.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "Kismet/GameplayStatics.h"
#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// AVerticalSliceCharacter

AVerticalSliceCharacter::AVerticalSliceCharacter()
{
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
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AVerticalSliceCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVerticalSliceCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVerticalSliceCharacter::MoveRight);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AVerticalSliceCharacter::Interact);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AVerticalSliceCharacter::initalise(AMech* mech)
{
	if (mech)
	{
		PlayerMech = mech;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Yay");
	}
	GetWorldTimerManager().SetTimer(InteractCheck, this, &AVerticalSliceCharacter::CheckInteract, 0.1, true);

	collParams.AddIgnoredActor(this);
}

void AVerticalSliceCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		EAxis::Type Axis = (climbing) ? EAxis::Z : EAxis::X;

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(Axis);
		AddMovementInput(Direction, Value);
	}
}

void AVerticalSliceCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
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

void AVerticalSliceCharacter::Interact()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Interact");
	if (!Mount())
	{
		TArray<FHitResult> hits;
		FVector start = GetActorLocation()+FVector(0,0,50);
		FVector end = start + (GetActorForwardVector());

		FCollisionShape CollShape = FCollisionShape::MakeSphere(InteractRange);

		bool isHit  = GetWorld()->SweepMultiByChannel(hits, start, end, FQuat(), ECollisionChannel::ECC_Visibility, CollShape, collParams);
		//GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility);
		//DrawDebugLine(GetWorld(), start, end, FColor::Emerald, false, 10.0f);
		if (isHit)
		{
			for (auto& hit : hits)
			{
				if (hit.bBlockingHit)
				{
					AInteractableVolume* intVol = Cast<AInteractableVolume>(hit.Actor);
					if (intVol)
					{
						intVol->activated = true;
					}
				}
			}
		}
	}
}

void AVerticalSliceCharacter::CheckInteract()
{
	TArray<FHitResult> hits;
	FVector start = GetActorLocation() + FVector(0, 0, 50);
	FVector end = start + (GetActorForwardVector());

	FCollisionShape CollShape = FCollisionShape::MakeSphere(InteractRange);

	bool isHit = GetWorld()->SweepMultiByChannel(hits, start, end, FQuat(), ECollisionChannel::ECC_Visibility, CollShape, collParams);
	//GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility);
	//DrawDebugLine(GetWorld(), start, end, FColor::Emerald, false, 10.0f);
	if (isHit)
	{
		for (auto& hit : hits)
		{
			if (hit.bBlockingHit)
			{
				AInteractableVolume* intVol = Cast<AInteractableVolume>(hit.Actor);
				if (intVol)
				{
					nearInteractableObject = true;
					InteractObjectLocation = intVol->GetActorLocation();
					return;
				}
			}
		}
	}
	InteractObjectLocation = FVector();
	nearInteractableObject = false;
}

bool AVerticalSliceCharacter::Mount()
{
	if (PlayerMech)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Woo");
		if (GetActorLocation().Equals(PlayerMech->GetActorLocation(), MountRange))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Range");
			AController* controller = GetController();
			AController* controller2 = PlayerMech->GetController();
			controller->UnPossess();
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			controller->Possess(Cast<APawn>(PlayerMech));
			PlayerMech->Mount();
			controller2->Destroy();
			GetWorldTimerManager().ClearTimer(InteractCheck);
			Destroy();
			return true;
		}
	}
	return false;
}

void AVerticalSliceCharacter::SetClimbing(bool newClimb, FVector Forward, FVector Up)
{
	climbing = newClimb;

	UCharacterMovementComponent* charMovement = GetCharacterMovement();

	charMovement->SetMovementMode((climbing) ? EMovementMode::MOVE_Flying : EMovementMode::MOVE_Walking);
	charMovement->bConstrainToPlane = climbing;
	charMovement->SetPlaneConstraintFromVectors(Forward, Up);
	charMovement->bOrientRotationToMovement = !climbing;
}
