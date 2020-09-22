// Fill out your copyright notice in the Description page of Project Settings.


#include "FlamethrowerBase.h"

#include "ArmorBase.h"
#include "Mech.h"

#include "Engine.h"

void AFlamethrowerBase::Shoot()
{
	if (ShootingTimer < SecondsBetweenShots) { return; }

	SecondsBetweenShots = 1 / ShotsPerSecond;

	if (CurrentClipSize <= 0 && IsMainGun) { return; }

	FlameShoot();

	ShootingTimer = 0.0f;

	Shooting = Automatic;
}

void AFlamethrowerBase::StopShoot()
{
	Super::StopShoot();

	if (shootingAnimation)
	{
		UAnimInstance* mechAnimInst = AttachedMech->GetMesh()->GetAnimInstance();

		if (mechAnimInst->Montage_IsPlaying(shootingAnimation))
		{
			mechAnimInst->Montage_Stop(0.1, shootingAnimation);
		}
	}
}

void AFlamethrowerBase::FlameTick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("fire")));
	auto IgnitedIterator = IgnitedMonsters.CreateConstIterator();
	while (IgnitedIterator)
	{
		if (IgnitedMonsters[IgnitedIterator.Key()].Value > IgnitedMonsters[IgnitedIterator.Key()].Key || IgnitedIterator.Key()->isDead)
		{
			auto DestroyedIt = IgnitedIterator;
			++IgnitedIterator;
			IgnitedMonsters.Remove(DestroyedIt.Key());
		}
		else
		{
			IgnitedIterator.Key()->DamageMonster(FlameTickDamage, IgnitedIterator.Key()->GetActorLocation(), FName());

			IgnitedMonsters[IgnitedIterator.Key()].Value++;

			++IgnitedIterator;
		}
	}
}

void AFlamethrowerBase::FlameShoot()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("pew")));

	CurrentClipSize--;

	if (shootingAnimation)
	{
		UAnimInstance* mechAnimInst = AttachedMech->GetMesh()->GetAnimInstance();

		if (!mechAnimInst->Montage_IsPlaying(shootingAnimation))
		{
			mechAnimInst->Montage_Play(shootingAnimation);
		}
	}

	FVector ConeSpreadDir = FVector();

	shotEnd.Empty();

	hitResults.Empty();
	FHitResult currHit;

	shotStart = Muzzle->GetComponentLocation();

	float Dist = 0;

	FVector CamLookLoc = AttachedMech->GetCameraLookLocation(MaxRange, Dist);

	//making a cone with line traces by changing where the lines end

	ConeSpreadDir = Muzzle->GetUpVector();
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = Muzzle->GetUpVector() * 0.5 + Muzzle->GetRightVector() * 0.5;
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = Muzzle->GetRightVector();
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = Muzzle->GetUpVector() * -0.5 + Muzzle->GetRightVector() * 0.5;
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = -Muzzle->GetUpVector();
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = Muzzle->GetUpVector() * -0.5 + Muzzle->GetRightVector() * -0.5;
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = -Muzzle->GetRightVector();
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = Muzzle->GetUpVector() * 0.5 + Muzzle->GetRightVector() * -0.5;
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = Muzzle->GetUpVector() * 0.5;
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = Muzzle->GetRightVector() * 0.5;
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = Muzzle->GetUpVector() * -0.5;
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = Muzzle->GetRightVector() * -0.5;
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	ConeSpreadDir = FVector();
	shotEnd.Add(CamLookLoc + (ConeSpreadDir * FlameRadius));

	TArray<FHitResult> CurrentHits;

	for(FVector End : shotEnd)
	{
		GetWorld()->LineTraceMultiByChannel(CurrentHits, shotStart, End, ECC_GameTraceChannel2, ignoredActors);
		DrawDebugLine(GetWorld(), shotStart, End, FColor::Emerald, false, 0.5f);

		hitResults.Append(CurrentHits);
	}

	TArray<AMonsterBase*> UniqueHitMonsters;

	for (FHitResult hit : hitResults)
	{
		if (hit.GetActor())
		{
			UArmorBase* ArmorPlate = Cast<UArmorBase>(hit.GetComponent());
			if(!ArmorPlate)
			{
				AMonsterBase* HitActor = Cast<AMonsterBase>(hit.GetActor());
				if (HitActor)
				{
					UniqueHitMonsters.AddUnique(HitActor);
				}
			}
		}
	}

	for (auto Monster : UniqueHitMonsters)
	{
		if (!HitMonsters.Contains(Monster))
		{
			HitMonsters.Add(Monster, 0);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NewMonster")));
		}
	}

	auto MonsterIterator = HitMonsters.CreateConstIterator();
	while (MonsterIterator)
	{
		if (!UniqueHitMonsters.Contains(MonsterIterator.Key()) || MonsterIterator.Key()->isDead)
		{
			auto DestroyedIt = MonsterIterator;
			++MonsterIterator;
			HitMonsters.Remove(DestroyedIt.Key());
		}
		else
		{
			MonsterIterator.Key()->DamageMonster(CalcDamage((Muzzle->GetComponentLocation() - MonsterIterator.Key()->GetActorLocation()).Size()), MonsterIterator.Key()->GetActorLocation(), FName());

			if (HitMonsters[MonsterIterator.Key()] > MaxHitsToIgnite)
			{
				IgniteMonster(MonsterIterator.Key());
			}
			else if ((HitMonsters[MonsterIterator.Key()] > MinHitsToIgnite))
			{
				if (FMath::FRandRange(0.0f, 1.0f) <= ChanceToIgnite)
				{
					IgniteMonster(MonsterIterator.Key());
				}
			}

			HitMonsters[MonsterIterator.Key()]++;

			++MonsterIterator;
		}
	}


	if (!GetWorldTimerManager().IsTimerActive(FlameTickHandle) && IgnitedMonsters.Num() > 0)
	{
		GetWorldTimerManager().SetTimer(FlameTickHandle, this, &AFlamethrowerBase::FlameTick, FlameTickRate);
	}
}

void AFlamethrowerBase::UpgradeFireDamage(int _Amount)
{
	FlameTickDamage = DefaultFlameTickDamage + FlameTickDamageIncrement;
}

void AFlamethrowerBase::IgniteMonster(AMonsterBase* Monster)
{
	if (IgnitedMonsters.Contains(Monster))
	{
		IgnitedMonsters[Monster].Value = 0;
	}
	else
	{
		IgnitedMonsters.Add(Monster, TPair<int, int>(FMath::RandRange(MinFlameTicks, MaxFlameTicks), 0));
	}
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
