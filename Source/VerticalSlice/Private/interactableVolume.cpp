// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "interactableVolume.h"

// Sets default values
AinteractableVolume::AinteractableVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxColl = CreateDefaultSubobject<UBoxComponent>(TEXT("boxColl"));

	SetRootComponent(boxColl);
}

// Called when the game starts or when spawned
void AinteractableVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AinteractableVolume::Tick(float DeltaTime)
{
}


