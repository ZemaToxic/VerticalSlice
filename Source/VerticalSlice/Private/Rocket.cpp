// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"

#include "MonsterBase.h"
#include "Mech.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine.h"

// Sets default values
ARocket::ARocket()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RocketCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RocketCollision"));
	SetRootComponent(Cast<USceneComponent>(RocketCollision));
	RocketCollision->OnComponentBeginOverlap.AddDynamic(this, &ARocket::BeginOverlap);

	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMoveRocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);

	RocketMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("RocketMove"));
}

// Sets default values
void ARocket::Init(FVector _LaunchLoc, FVector _AimLoc, float _Damage, float _ExplosionRadius)
{
	LaunchLoc = _LaunchLoc;
	AimLoc = _AimLoc;
	Damage = _Damage;
	ExplosionRadius = _ExplosionRadius;

	FVector NormalLaunchDir = LaunchLoc - GetActorLocation();
	NormalLaunchDir.Normalize();
	SetActorRotation(FQuat::FindBetweenNormals(FVector::UpVector, NormalLaunchDir));

	FVector NormalAimDir = AimLoc - LaunchLoc;
	NormalAimDir.Normalize();
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();

	FVector NormalLaunchDir = LaunchLoc - GetActorLocation();
	NormalLaunchDir.Normalize();
	SetActorRotation(FQuat::FindBetweenNormals(FVector::UpVector, NormalLaunchDir));

	FVector NormalAimDir = AimLoc - LaunchLoc;
	NormalAimDir.Normalize();

	
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([=]
		{
			Explode();
		});

	GetWorldTimerManager().SetTimer(FlyTime, TimerCallback, MaxFlyTime, false);
}

void ARocket::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ARocket>(OtherActor) || Cast<AMech>(OtherActor)) { return; }
	GetWorldTimerManager().ClearTimer(FlyTime);
	Explode();
}

void ARocket::Explode()
{
	// create tarray for hit results
	TArray<FHitResult> OutHits;

	// start and end locations
	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = SweepStart + FVector(0.1, 0, 0);

	// create a collision sphere
	FCollisionShape ExplosionColl = FCollisionShape::MakeSphere(ExplosionRadius);

	// draw collision box
	DrawDebugSphere(GetWorld(), SweepStart, ExplosionRadius, 30, FColor::Purple, false, 1.0f);

	// check if something got hit in the sweep
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel1, ExplosionColl);

	if (isHit)
	{
		TArray<AMonsterBase*> HitMonsters;
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			AMonsterBase* HitActor = Cast<AMonsterBase>(Hit.GetActor());
			if (HitActor)
			{
				if (!(HitMonsters.Contains(HitActor)))
				{
					HitActor->DamageMonster(Damage, HitActor->GetActorLocation(), Hit.BoneName);
				}
			}
		}
	}
	Destroy();
}

// Called every frame
void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Equals(LaunchLoc, 100))
	{
		float speed = RocketMove->Velocity.Size();
		RocketMove->Velocity.Normalize();
		RocketMove->Velocity *= (speed - AccelerationAmount * DeltaTime > SpeedForTurning) ? speed - AccelerationAmount * DeltaTime : SpeedForTurning;
		if (RocketMove->Velocity.Size() == SpeedForTurning)
		{
			AtLaunch = true;
		}
	}

	if (AtLaunch)
	{
		FVector NormalAimDir = AimLoc - GetActorLocation();
		NormalAimDir.Normalize();
		RotGoal = FQuat::FindBetweenNormals(FVector::UpVector, NormalAimDir);

		if (GetActorQuat().Equals(RotGoal, RotationTolerance))
		{
			float speed = RocketMove->Velocity.Size();
			RocketMove->Velocity.Normalize();
			RocketMove->Velocity *= (speed - AccelerationAmount * DeltaTime > SpeedForLaunch) ? speed + AccelerationAmount * DeltaTime: SpeedForLaunch;
		}
		else
		{
			SetActorRotation(FQuat::Slerp(GetActorQuat(), RotGoal, RotationInterp));
		}	
	}

	float speed = RocketMove->Velocity.Size();

	RocketMove->Velocity = GetActorUpVector();
	RocketMove->Velocity *= speed;
}

