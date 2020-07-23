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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom Variables | Gameplay")
		bool climbing = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom Variables | Gameplay")
		bool nearInteractableObject = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Variables | Gameplay")
		FVector InteractObjectLocation = FVector();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom Variables | Gameplay")
		FTimerHandle InteractCheck;

	void initalise(class AMech* mech);

private:
	UPROPERTY(EditAnywhere, Category = "Custom Variables | Gameplay")
		float MountRange = 300;

	UPROPERTY(EditAnywhere, Category = "Custom Variables | Gameplay")
		float InteractRange = 300;

	UPROPERTY(VisibleAnywhere, Category = "Custom Variables | Watchables")
		class AMech* PlayerMech = 0;

	FCollisionQueryParams collParams;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Interact();

	void CheckInteract();

	UFUNCTION(BlueprintNativeEvent, category = "Custom | End")
	bool Mount();

	UFUNCTION(BlueprintCallable, category = "Custom | Climbing")
		void SetClimbing(bool newClimb, FVector Forward, FVector Up);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void SetVisible(bool visibility);
};

