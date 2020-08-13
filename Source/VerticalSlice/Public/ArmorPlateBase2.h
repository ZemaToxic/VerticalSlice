// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ArmorPlateBase2.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UArmorPlateBase2 : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void DamagePlate(float _Damage, FVector _HitLoc);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void DestroyPlate();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ResetPlate();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate")
		float PlateHealth = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate")
		float MaxPlateHealth = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate")
		bool IsDestroyed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate")
		class UStaticMeshComponent* SMRef;

	// Array of strings that will be added to the weakspots of the monster once the plate is destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate")
		TArray<FString> ExposedWeakspots;
};
