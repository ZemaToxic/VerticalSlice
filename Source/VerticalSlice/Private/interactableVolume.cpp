// Fill out your copyright notice in the Description page of Project Settings.

#include "interactableVolume.h"

#include "Components/BoxComponent.h"

// Sets default values
AInteractableVolume::AInteractableVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxColl = CreateDefaultSubobject<UBoxComponent>(TEXT("boxColl"));

	SetRootComponent(boxColl);

	boxColl->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AInteractableVolume::BeginPlay()
{
	Super::BeginPlay();
	
}


bool AInteractableVolume::GetActivated()
{
	return activated;
}

void AInteractableVolume::Activate_Implementation()
{
	activated = true;
}

void AInteractableVolume::Reset_Implementation()
{
	activated = false;
}
