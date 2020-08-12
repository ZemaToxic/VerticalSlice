// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorPlateBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERTICALSLICE_API UArmorPlateBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArmorPlateBase();

	UFUNCTION(BlueprintCallable)
		void DamagePlate(float _Damage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate")
		float PlateHealth = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate")
		bool IsDestroyed = false;

	// Array of strings that will be added to the weakspots of the monster once the plate is destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate")
		TArray<FString> ExposedWeakspots;

	// Reference to the static mesh component of the plate, will be disabled once the plate is destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate")
		class UStaticMeshComponent* StaticMeshRef;

	UFUNCTION(BlueprintCallable)
		void DestroyPlate();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
