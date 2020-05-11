// Fill out your copyright notice in the Description page of Project Settings.

#include <vector>

#include "GunBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

#include "Engine.h"

// Sets default values
AGunBase::AGunBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);
	GunMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	Muzzle = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleLoc"));
	Muzzle->SetupAttachment(GunMesh);
	Muzzle->SetHiddenInGame(true);
}

void AGunBase::Shoot()
{
	SecondsBetweenShots = 1 / ShotsPerSecond;

	ShootRaycasts();

	ShootingTimer = 0.0f;

	Shooting = Automatic;
}

void AGunBase::StopShoot()
{
	ShootingTimer = 0.0f;

	Shooting = false;
}

void AGunBase::ShootRaycasts_Implementation()
{
	FVector gunDir = Muzzle->GetForwardVector();

	FVector shotStart = Muzzle->GetComponentLocation();

	std::vector<FHitResult> hitResults;
	FHitResult currHit;

	for (int i = 0; i < BulletsPerShot; i++)
	{
		FVector randomSpreadVec = FVector(0, FMath::FRandRange(LowerSpread.X, UpperSpread.X), FMath::FRandRange(LowerSpread.Y, UpperSpread.Y));

		FVector shotEnd = shotStart + (gunDir * Range) + (Muzzle->GetUpVector() * FMath::FRandRange(LowerSpread.Y, UpperSpread.Y)) + (Muzzle->GetRightVector() * FMath::FRandRange(LowerSpread.X, UpperSpread.X));

		GetWorld()->LineTraceSingleByChannel(currHit, shotStart, shotEnd, ECollisionChannel::ECC_Visibility);

		DrawDebugLine(GetWorld(), shotStart, shotEnd, FColor::Emerald, false, 0.5f);

		hitResults.push_back(currHit);
	}

	//evaluate hit result
}

void AGunBase::Tick(float DeltaTime)
{
	if (Shooting)
	{
		if (ShootingTimer >= SecondsBetweenShots)
		{
			ShootRaycasts();

			ShootingTimer = 0.0f;
		}
		ShootingTimer += DeltaTime;
	}
}

