// Fill out your copyright notice in the Description page of Project Settings.


#include "DropsBase.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ADropsBase::ADropsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(DropMesh);
}

// Called when the game starts or when spawned
void ADropsBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADropsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

