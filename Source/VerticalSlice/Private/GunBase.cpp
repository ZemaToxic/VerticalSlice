// Fill out your copyright notice in the Description page of Project Settings.

#include "GunBase.h"

#include "Mech.h"
#include "MonsterBase.h"
#include "ArmorBase.h"

#include <vector>

#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

#include "NiagaraFunctionLibrary.h"
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
	if (ShootingTimer < SecondsBetweenShots) { return; }

	SecondsBetweenShots = 1 / ShotsPerSecond;

	if (CurrentMagsize <= 0 && usesBullets) {
		if (AttachedMech)
		{
			AttachedMech->Reload();
		}
		return;
	}

	ShootRaycasts();

	ShootingTimer = 0.0f;

	Shooting = Automatic;
}

void AGunBase::StopShoot()
{
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

	shotEnd.Empty();

	hitResults.Empty();
	FHitResult currHit;

	int j = 0;

	for (int i = 0; i < BulletsPerShot; i++)
	{
		shotStart = Muzzle->GetComponentLocation();

		shotEnd.Add((FVector)0);

		float randY = FMath::FRandRange(LowerSpread.Y, UpperSpread.Y);
		float randX = FMath::FRandRange(LowerSpread.X, UpperSpread.X);
		FVector RandomSpreadDirection = (AttachedMech->GetFollowCamera()->GetUpVector() * randY) + (AttachedMech->GetFollowCamera()->GetRightVector() * randX);
		
		
		float Dist = 0;

		FVector CamLookLoc = AttachedMech->GetCameraLookLocation(MaxRange, Dist);

		shotEnd[j] = CamLookLoc + (RandomSpreadDirection * (Dist/1000));

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f, %f, %f"), randY, randX, Dist));

		GetWorld()->LineTraceSingleByChannel(currHit, shotStart, shotEnd[j], ECC_Visibility, ignoredActors);

		//DrawDebugLine(GetWorld(), shotStart, shotEnd[j], FColor::Emerald, false, 0.5f);

		if (currHit.bBlockingHit)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Ono");
			shotEnd.Pop();
			j--;
			hitResults.Add(currHit);
		}
		j++;
	}

	if (hitResults.Num() == 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Ono");
		return;
	}

	for (auto& hit : hitResults)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *(hit.Actor.Get()->GetName())));

		UArmorBase* ArmorPlate = Cast<UArmorBase>(hit.GetComponent());

		if (ArmorPlate)
		{
			if (DestroysArmourPlate)
			{
				ArmorPlate->DestroyPlate();
			}
			else
			{
				ArmorPlate->DamagePlate(CalcDamage((Muzzle->GetComponentLocation() - hit.GetComponent()->GetComponentLocation()).Size())/10, hit.Location);
			}
			
		}
		else
		{
			AMonsterBase* HitActor = Cast<AMonsterBase>(hit.GetActor());
			if (HitActor)
			{
				HitActor->DamageMonster(CalcDamage((Muzzle->GetComponentLocation() - hit.GetComponent()->GetComponentLocation()).Size()), hit.Location, hit.BoneName);
				if (HitPS)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Yay");
					//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPS, hit.Location);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitPS, hit.Location);
				}
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
		SecondsBetweenShots = 0;
	}
}

void AGunBase::UpgradeDamage(float _Multiplier)
{
}

void AGunBase::UpgradeClipSize(float _Multiplier)
{
}

void AGunBase::UpgradeBulletsPerShot(float _Multiplier)
{
}

void AGunBase::UpgradeRange(float _Multiplier)
{
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

float AGunBase::CalcDamage(float Dist)
{
	float RandDamage = FMath::FRandRange(Damage - (DamageRange / 2), Damage + (DamageRange / 2));

	float OptimalDist = OptimalRangePercent * MaxRange;

	float NormalisedDist = FMath::Clamp(((Dist - OptimalDist) / (MaxRange - OptimalDist)), 0.0f, 1.0f);
	float Falloff = 0;
	if (FalloffCurve > 0 && FalloffCurve != 1)
	{
		Falloff = DamageFalloff * ((powf(FalloffCurve, NormalisedDist) - 1) / (FalloffCurve - 1));
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), DamageFalloff));

	return RandDamage - Falloff;
}

void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShootingTimer < SecondsBetweenShots)
	{
		ShootingTimer += DeltaTime;
	}
	else if (Shooting)
	{
		if (CurrentMagsize <= 0) {
			Shooting = false;
			return;
		}
		ShootRaycasts();
		ShootingTimer = 0.0f;
	}
	
}

