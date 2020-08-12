// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorPlateBase.h"

#include "Components/StaticMeshComponent.h"
#include "Engine.h"

#include "MonsterBase.h"

// Sets default values for this component's properties
UArmorPlateBase::UArmorPlateBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


// Called when the game starts
void UArmorPlateBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UArmorPlateBase::DamagePlate(float _Damage)
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

void UArmorPlateBase::DestroyPlate()
{
	IsDestroyed = true;
	if (IsValid(StaticMeshRef))
	{
		StaticMeshRef->SetActive(false);
	}
	AMonsterBase* MonsterRef = Cast<AMonsterBase>(UActorComponent::GetOwner());
	for (auto& weakspot : ExposedWeakspots)
	{
		MonsterRef->WeakspotsScript.Add(weakspot);
	}
}

// Called every frame
void UArmorPlateBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

