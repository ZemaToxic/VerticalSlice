// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"

#include "MonsterBase.h"

#include "DropsBase.generated.h"

class UCurveFloat;

UCLASS()
class VERTICALSLICE_API ADropsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropsBase();

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Components")
		class USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UStaticMeshComponent* DropMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class USphereComponent* PickupSphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class USphereComponent* MagnetSphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
		MonsterDrops DropType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		FRotator RotationRate;



	// FUNCTIONS
	UFUNCTION()
		void TimelineProgress(float Value);

	UFUNCTION()
		void FloatAnim();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		bool CheckInRange();

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* CurveFloat;

	UPROPERTY()
		FVector StartLoc;

	UPROPERTY()
		FVector EndLoc;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		float ZOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
