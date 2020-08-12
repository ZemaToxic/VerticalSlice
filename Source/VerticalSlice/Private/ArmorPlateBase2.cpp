// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmorPlateBase2.h"

#include "MonsterBase.h"

#include "Engine.h"

void UArmorPlateBase2::DamagePlate_Implementation(float _Damage)
{
	if (!IsDestroyed)
	{
		PlateHealth -= _Damage;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
		}
		if (PlateHealth <= 0.0f)
		{
			DestroyPlate();
		}
	}
}

void UArmorPlateBase2::DestroyPlate_Implementation()
{
	IsDestroyed = true;
	// TODO: Make SM invisible
	AMonsterBase* MonsterRef = Cast<AMonsterBase>(UActorComponent::GetOwner());
	for (auto& weakspot : ExposedWeakspots)
	{
		MonsterRef->WeakspotsScript.Add(weakspot);
	}
}
