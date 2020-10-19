// Fill out your copyright notice in the Description page of Project Settings.


#include "FlamethrowerBase.h"

#include "ArmorBase.h"
#include "Mech.h"
#include "EnergyBlast.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine.h"

void AFlamethrowerBase::Shoot_Implementation()
{
	if (ShootingTimer < SecondsBetweenShots) { return; }

	SecondsBetweenShots = 1 / ShotsPerSecond;

	if (CurrentClipSize <= 0) { return; }

	FlameShoot();

	ShootingTimer = 0.0f;

	Shooting = Automatic;
}

void AFlamethrowerBase::StopShoot_Implementation()
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

	if (CurrentShootingEffect)
	{
		CurrentShootingEffect->Deactivate();
	}
}

void AFlamethrowerBase::FlameTick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("fire")));
	auto IgnitedIterator = IgnitedMonsters.CreateConstIterator();
	while (IgnitedIterator)
	{
		if (!IsValid(IgnitedIterator.Key())||IgnitedMonsters[IgnitedIterator.Key()].CurrentTicks > IgnitedMonsters[IgnitedIterator.Key()].MaxTicks || IgnitedIterator.Key()->isDead)
		{
			auto DestroyedIt = IgnitedIterator;
			++IgnitedIterator;

			if (IsValid(IgnitedMonsters[DestroyedIt.Key()].Fire))
			{
				IgnitedMonsters[DestroyedIt.Key()].Fire->DestroyInstance();
			}
			IgnitedMonsters.Remove(DestroyedIt.Key());
		}
		else
		{
			IgnitedIterator.Key()->DamageMonster(FlameTickDamage, IgnitedIterator.Key()->GetActorLocation(), FName(), KnockbackForce);

			IgnitedMonsters[IgnitedIterator.Key()].CurrentTicks++;

			++IgnitedIterator;
		}
	}

	if (IgnitedMonsters.Num() > 0)
	{
		GetWorldTimerManager().SetTimer(FlameTickHandle, this, &AFlamethrowerBase::FlameTick, FlameTickRate);
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

	if (!CurrentShootingEffect && ShootingEffect)
	{
		CurrentShootingEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(ShootingEffect, Cast<USceneComponent>(Muzzle), FName(), FVector(), FRotator(), EAttachLocation::SnapToTarget, false);
	}
	else if (CurrentShootingEffect)
	{
		CurrentShootingEffect->Activate();
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
		//DrawDebugLine(GetWorld(), shotStart, End, FColor::Emerald, false, 0.5f);

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
				AEnergyBlast* EnergyBlastActor = Cast<AEnergyBlast>(hit.GetActor());
				if (HitActor)
				{
					UniqueHitMonsters.AddUnique(HitActor);
				}
				else if (EnergyBlastActor)
				{
					EnergyBlastActor->StartExplosion();
				}
			}
		}
	}

	for (auto Monster : UniqueHitMonsters)
	{
		if (!HitMonsters.Contains(Monster))
		{
			HitMonsters.Add(Monster, 0);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NewMonster")));
		}
	}

	auto MonsterIterator = HitMonsters.CreateConstIterator();
	while (MonsterIterator)
	{
		if (!IsValid(MonsterIterator.Key())||!UniqueHitMonsters.Contains(MonsterIterator.Key()) || MonsterIterator.Key()->isDead)
		{
			auto DestroyedIt = MonsterIterator;
			++MonsterIterator;
			HitMonsters.Remove(DestroyedIt.Key());
		}
		else
		{
			MonsterIterator.Key()->DamageMonster(CalcDamage((Muzzle->GetComponentLocation() - MonsterIterator.Key()->GetActorLocation()).Size()), MonsterIterator.Key()->GetActorLocation(), FName(), KnockbackForce);

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
		IgnitedMonsters[Monster].CurrentTicks = 0;
	}
	else
	{
		UNiagaraComponent* OnFire = 0;
		if (OnFireEffect)
		{
			OnFire = UNiagaraFunctionLibrary::SpawnSystemAttached(OnFireEffect, Cast<USceneComponent>(Monster->GetRootComponent()), FName(), FVector(), FRotator(), EAttachLocation::SnapToTarget, false);
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *OnFire->GetName()));
		
		IgnitedMonsters.Add(Monster, FOnFire(FMath::RandRange(MinFlameTicks, MaxFlameTicks), 0, OnFire));
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
			StopShoot();
			return;
		}
		FlameShoot();
		ShootingTimer = 0.0f;
	}
	Muzzle->SetWorldRotation(AttachedMech->GetControlRotation());
}
