// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interactableVolume.generated.h"

UCLASS()
class VERTICALSLICE_API AInteractableVolume : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* boxColl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Checking, meta = (AllowPrivateAccess = "true"))
		int id = 0;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Checking)
		bool activated = false;
	
public:	
	// Sets default values for this actor's properties
	AInteractableVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
