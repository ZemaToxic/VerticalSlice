// Fill out your copyright notice in the Description page of Project Settings.

#include "GunBase.h"

#include "Mech.h"
#include "MonsterBase.h"

#include <vector>

#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Engine.h"

// Sets default values
AGunBase::AGunBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);
	GunMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	Muzzle = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleLoc"));
	Muzzle->SetupAttachment(GunMesh);
	Muzzle->SetHiddenInGame(true);
}

void AGunBase::init(AMech* mech)
{
	if (mech)
	{
		AttachedMech = mech;
	}
}

void AGunBase::Shoot()
{
	SecondsBetweenShots = 1 / ShotsPerSecond;

	if (CurrentMagsize <= 0) {
		Shooting = false;
		return;
	}

	ShootRaycasts();

	ShootingTimer = 0.0f;

	Shooting = Automatic;
}

void AGunBase::StopShoot()
{
	ShootingTimer = 0.0f;

	Shooting = false;
}

void AGunBase::setShootAnim(UAnimMontage* newAnim)
{
	if (newAnim)
	{
		shootingAnimation = newAnim;
	}
}

void AGunBase::ShootRaycasts_Implementation()
{
	CurrentMagsize--;

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

	FVector gunDir = Muzzle->GetForwardVector();

	FVector shotStart = Muzzle->GetComponentLocation();

	std::vector<FHitResult> hitResults;
	FHitResult currHit;

	for (int i = 0; i < BulletsPerShot; i++)
	{
		FVector randomSpreadVec = FVector(0, FMath::FRandRange(LowerSpread.X, UpperSpread.X), FMath::FRandRange(LowerSpread.Y, UpperSpread.Y));

		FVector shotEnd = shotStart + (gunDir * Range) + (Muzzle->GetUpVector() * FMath::FRandRange(LowerSpread.Y, UpperSpread.Y)) + (Muzzle->GetRightVector() * FMath::FRandRange(LowerSpread.X, UpperSpread.X));

		GetWorld()->LineTraceSingleByChannel(currHit, shotStart, shotEnd, ECollisionChannel::ECC_Visibility, ignoredActors);

		//DrawDebugLine(GetWorld(), shotStart, shotEnd, FColor::Emerald, false, 0.5f);

		if (currHit.bBlockingHit)
		{
			hitResults.push_back(currHit);
		}
	}

	if (hitResults.size() == 0)
	{
		return;
	}

	for (auto& hit : hitResults)
	{
		AMonsterBase* HitActor = Cast<AMonsterBase>(hit.GetActor());
		if (HitActor)
		{
			HitActor->DamageMonster(Damage, hit.Location, hit.BoneName);
			if (HitPS && Cast<USceneComponent>(hit.GetComponent()))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Yay");
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPS, hit.Location);
			}
		}
	}
}

void AGunBase::Reload(int& ammoPool)
{
	if (CurrentMagsize <= MaxMagsize && !Shooting)
	{
		if (ammoPool - (MaxMagsize - CurrentMagsize) >= 0)
		{
			ammoPool -= MaxMagsize - CurrentMagsize;
			CurrentMagsize = MaxMagsize;
		}
		else
		{
			CurrentMagsize += ammoPool;
			ammoPool = 0;
		}
	}
}

void AGunBase::Upgrade(GunUpgrades upgrade)
{
	switch (upgrade)
	{
	case GunUpgrades::BetterFireRate:
		ShotsPerSecond *= 1.2;
		break;
	case GunUpgrades::FasterReload:
		if (AttachedMech)
		{
			AttachedMech->Upgrade(MechUpgrades::FasterReload);
		}
		break;
	case GunUpgrades::BetterDamage:
		Damage *= 1.5;
		break;
	default:
		break;
	}
	LastGunUpgrade = upgrade;
}

void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	ignoredActors.AddIgnoredActor(this);
	AActor* player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (player)
	{
		ignoredActors.AddIgnoredActor(player);
	}
}

void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Shooting)
	{
		if (ShootingTimer >= SecondsBetweenShots)
		{
			if (CurrentMagsize <= 0) {
				Shooting = false;
				return;
			}
			ShootRaycasts();
			ShootingTimer = 0.0f;
		}
		ShootingTimer += DeltaTime;
	}
}

