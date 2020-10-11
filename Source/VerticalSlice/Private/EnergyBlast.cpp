// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyBlast.h"

// Sets default values
AEnergyBlast::AEnergyBlast()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnergyBlast::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnergyBlast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnergyBlast::StartExplosion_Implementation()
{
}

