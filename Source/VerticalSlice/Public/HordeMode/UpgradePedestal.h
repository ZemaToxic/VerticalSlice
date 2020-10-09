// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GM_HordeMode.h"
#include "../VerticalSliceCharacter.h"
#include "../Mech.h"
#include "../interactableVolume.h"
#include "UpgradePedestal.generated.h"

class AGM_HordeMode;

UCLASS()
class VERTICALSLICE_API AUpgradePedestal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUpgradePedestal();
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* UpgradeMesh;
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
		AInteractableVolume* Interactable;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckPurchase();
	void UpgradeMech(int _iChoosenUpgade);

	void SetUpgrade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float fUpgradeCost;
	int iCurrentUpgrade;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
