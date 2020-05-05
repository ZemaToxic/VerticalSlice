// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

#include "Engine.h"

// Sets default values
AGunBase::AGunBase()
{
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
	FVector gunDir = Muzzle->GetForwardVector();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Gun Direction: %s"), *gunDir.ToString()));
	
	FHitResult hit;

	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + (gunDir * Range), ECollisionChannel::ECC_Visibility);

	DrawDebugLine(GetWorld(), Muzzle->GetComponentLocation(), Muzzle->GetComponentLocation() + (gunDir * Range), FColor::Emerald, false, 0.5f);
}

void AGunBase::StopShoot()
{
	Shooting = false;
}

