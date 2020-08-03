// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interactableVolume.generated.h"

UCLASS()
class VERTICALSLICE_API AInteractableVolume : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* boxColl;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Checking, meta = (AllowPrivateAccess = "true"))
		int id = 0;

	bool activated = false;

public:

	UFUNCTION(BlueprintPure, Category = "Checking")
		bool GetActivated();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Checking")
		void Activate();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Checking")
		void Reset();
public:	
	// Sets default values for this actor's properties
	AInteractableVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
