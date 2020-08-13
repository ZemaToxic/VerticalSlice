// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorBase.h"

#include "MonsterBase.h"

#include "Engine.h"
#include "Components/StaticMeshComponent.h"

/*	reduces the health of the plate if it has not been destroyed, if it's health is 
	reduced <=0 DestroyPlate is called
*/
void UArmorBase::DamagePlate_Implementation(float _Damage, FVector _HitLoc)
{
	if (!IsDestroyed)
	{
		PlateHealth -= _Damage;
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
		}*/
		if (PlateHealth <= 0.0f)
		{
			DestroyPlate();
		}
	}
}

// sets IsDestroyed to true and makes SMRef invisible and ignores all collision
void UArmorBase::DestroyPlate_Implementation()
{
	IsDestroyed = true;

	AMonsterBase* MonsterRef = Cast<AMonsterBase>(UActorComponent::GetOwner());
	for (auto& weakspot : ExposedWeakspots)
	{
		MonsterRef->WeakspotsScript.Add(weakspot);
	}

	// Set static mesh to invisible and ignore all collision
	//SetVisibility(false, false);
	SMRef->SetVisibility(false, false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void UArmorBase::ResetPlate_Implementation()
{
	// Resetting variables
	IsDestroyed = false;
	PlateHealth = MaxPlateHealth;

	AMonsterBase* MonsterRef = Cast<AMonsterBase>(UActorComponent::GetOwner());
	for (auto& weakspot : ExposedWeakspots)
	{
		MonsterRef->WeakspotsScript.Remove(weakspot);
	}

	// Set static mesh to visible and block collision for camera
	//SetVisibility(true, false);
	SMRef->SetVisibility(true, false);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

