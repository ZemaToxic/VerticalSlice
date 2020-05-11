// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ControllerMonster.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API AControllerMonster : public AAIController
{
	GENERATED_BODY()

public:
	AControllerMonster();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* _pawn) override;

	virtual void Tick(float _deltaSeconds) override;

	//UFUNCTION()
	//void OnPawnDetected(TArray<AActor*> DetectedPawns);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRadius = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius = AISightRadius + 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	class UAISenseConfig_Sight* SightConfig;

};
