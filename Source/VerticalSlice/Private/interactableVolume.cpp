// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "interactableVolume.h"

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


