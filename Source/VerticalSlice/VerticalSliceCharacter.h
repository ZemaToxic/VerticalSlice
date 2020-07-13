// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VerticalSliceCharacter.generated.h"

UCLASS(config=Game)
class AVerticalSliceCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AVerticalSliceCharacter();

	// used for testing Workshop UI. Can remove once we have an inventory system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	float Cash = 5000;

	UPROPERTY(EditAnywhere, Category = "Custom Variables | Gameplay")
		float MountRange = 1000;

	UPROPERTY(VisibleAnywhere, Category = "Custom Variables | Watchables")
	class AMech* PlayerMech = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom Variables | Gameplay")
		bool climbing = false;

	void initalise(class AMech* mech);

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Interact();

	bool Mount();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

