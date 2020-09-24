// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketLauncher.h"

#include "Mech.h"
#include "Rocket.h"
#include "Components/ArrowComponent.h"


void ARocketLauncher::Shoot()
{
	if (ShootingTimer < SecondsBetweenShots) { return; }

	SecondsBetweenShots = 1 / ShotsPerSecond;

	if (CurrentClipSize <= 0) { return; }

	ShootRocket();

	ShootingTimer = 0.0f;

	Shooting = Automatic;
}

void ARocketLauncher::ShootRocket()
{
	if (shootingAnimation)
	{
		UAnimInstance* mechAnimInst = AttachedMech->GetMesh()->GetAnimInstance();

		if (!mechAnimInst->Montage_IsPlaying(shootingAnimation))
		{
			mechAnimInst->Montage_Play(shootingAnimation);
		}
		else
		{
			mechAnimInst->Montage_SetPosition(shootingAnimation, 0.0f);
		}
	}

	if (RocketClass)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ARocket* CurrentRocket = GetWorld()->SpawnActor<ARocket>(RocketClass, Muzzle->GetComponentTransform(), spawnParams);
		if (CurrentRocket)
		{
			float Dist;
			CurrentRocket->Init(Muzzle->GetComponentLocation() + (AttachedMech->GetActorUpVector() * LaunchDistance), AttachedMech->GetCameraLookLocation(1000000, Dist), CalcDamage(0), ExplosionRadius);
			CurrentClipSize--;
		}
	}
}

void ARocketLauncher::UpgradeExplosionRadius(int _Amount)
{
	ExplosionRadius = ExplosionRadiusDefault + ExplosionRadiusUpgrade * _Amount;
}

void ARocketLauncher::Tick(float DeltaTime)
{
	if (ShootingTimer < SecondsBetweenShots)
	{
		ShootingTimer += DeltaTime;
	}
	else if (Shooting)
	{
		if (CurrentClipSize <= 0) {
			StopShoot();
			return;
		}
		ShootRocket();
		ShootingTimer = 0.0f;
	}
}
