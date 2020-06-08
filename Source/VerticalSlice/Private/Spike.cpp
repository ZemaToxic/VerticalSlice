// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASpike::ASpike()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("Root");

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>("Capsule");

	SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpikeMesh");
	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	SetRootComponent(CapsuleCollider);

	CapsuleCollider->InitCapsuleSize(15.f, 110.f);
}

// Called when the game starts or when spawned
void ASpike::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

