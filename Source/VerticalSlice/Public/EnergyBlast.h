// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnergyBlast.generated.h"

UCLASS()
class VERTICALSLICE_API AEnergyBlast : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnergyBlast();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyBlast")
		float DestroyAfterTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyBlast")
		float ExplosionEndRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyBlast")
		float Damage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyBlast")
		float ExplosionBeginRadius = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "EnergyBlast")
		bool IsExploding = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the Energy Blast should explode, implemented in BP
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StartExplosion();
};
