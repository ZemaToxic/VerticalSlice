// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spike.generated.h"

UCLASS()
class VERTICALSLICE_API ASpike : public AActor
{
	GENERATED_BODY()


	
public:	
	// Sets default values for this actor's properties
	ASpike();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	class USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* SpikeMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UCapsuleComponent* CapsuleCollider;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UProjectileMovementComponent* ProjMovement;

	UPROPERTY(EditAnywhere, Category = "Watchables")
	float speed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
