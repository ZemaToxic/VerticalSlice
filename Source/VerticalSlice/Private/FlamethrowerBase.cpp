// Fill out your copyright notice in the Description page of Project Settings.


#include "FlamethrowerBase.h"

#include "Mech.h"
#include "Engine.h"

void AFlamethrowerBase::Shoot()
{
	if (ShootingTimer < SecondsBetweenShots) { return; }

	SecondsBetweenShots = 1 / ShotsPerSecond;

	if (CurrentClipSize <= 0 && usesBullets) {
		if (AttachedMech)
		{
			AttachedMech->Reload();
		}
		return;
	}

	FlameShoot();

	ShootingTimer = 0.0f;

	Shooting = Automatic;
}

void AFlamethrowerBase::FlameTick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("fire")));
}

void AFlamethrowerBase::FlameShoot()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("pew")));
	FlameTick();
}

void AFlamethrowerBase::Tick(float DeltaTime)
{
	if (ShootingTimer < SecondsBetweenShots)
	{
		ShootingTimer += DeltaTime;
	}
	else if (Shooting)
	{
		if (CurrentClipSize <= 0) {
			Shooting = false;
			return;
		}
		FlameShoot();
		ShootingTimer = 0.0f;
	}

}
