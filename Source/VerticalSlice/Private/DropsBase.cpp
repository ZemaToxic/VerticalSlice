// Fill out your copyright notice in the Description page of Project Settings.


#include "DropsBase.h"
#include "MonsterBase.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ADropsBase::ADropsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComp);

	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	DropMesh->AttachTo(RootComponent);

	PickupSphereComp = CreateDefaultSubobject<USphereComponent>("PickupRadius");
	PickupSphereComp->SetupAttachment(RootComponent);
	PickupSphereComp->InitSphereRadius(400.0f);

	MagnetSphereComp = CreateDefaultSubobject<USphereComponent>("MagnetRadius");
	MagnetSphereComp->SetupAttachment(RootComponent);
	MagnetSphereComp->InitSphereRadius(2000.0f);
	MagnetSphereComp->ShapeColor = FColor::Green;

	RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	RotationSpeed = 1.0f;
}

void ADropsBase::TimelineProgress(float Value)
{
	if (!CheckInRange())
	{
		FVector NewLoc = FMath::Lerp(StartLoc, EndLoc, Value);
		SetActorLocation(NewLoc);
	}
	else // set the new start and end locations in case the player leaves the magnet radius
	{
		StartLoc = EndLoc = GetActorLocation();
		EndLoc.Z += ZOffset;
	}
}

void ADropsBase::FloatAnim()
{
	// floating functionality using timelines
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(true);

		StartLoc = EndLoc = GetActorLocation();
		EndLoc.Z += ZOffset;

		CurveTimeline.PlayFromStart();
	}
}

// Called when the game starts or when spawned
void ADropsBase::BeginPlay()
{
	Super::BeginPlay();
	
	FloatAnim();
}

// Function used to check if the drop is within range of the player using the MagnetSphereComp radius
bool ADropsBase::CheckInRange()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	float DistToPlayer = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());

	return (DistToPlayer<=MagnetSphereComp->GetScaledSphereRadius());
}

// Called every frame
void ADropsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);

	AddActorLocalRotation(RotationRate * DeltaTime * RotationSpeed);
}

