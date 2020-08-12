// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmorPlateBase2.h"

#include "MonsterBase.h"

#include "Engine.h"

void UArmorPlateBase2::DamagePlate_Implementation(float _Damage, FVector _HitLoc)
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

void UArmorPlateBase2::DestroyPlate_Implementation()
{
	IsDestroyed = true;

	AMonsterBase* MonsterRef = Cast<AMonsterBase>(UActorComponent::GetOwner());
	for (auto& weakspot : ExposedWeakspots)
	{
		MonsterRef->WeakspotsScript.Add(weakspot);
	}

	// Set static mesh to invisible and ignore all collision
	SetVisibility(false, false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void UArmorPlateBase2::ResetPlate_Implementation()
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
	SetVisibility(true, false);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}
