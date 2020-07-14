// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
//#include "AIControllerBase.h"
#include "DropsBase.h"


// Sets default values
AMonsterBase::AMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	//AAIControllerBase* AIController = Cast<AAIControllerBase>(GetController());

	health = maxHealth;
	
}

//void AMonsterBase::DropItems_Implementation()
//{
//	// make sure the DropTable array is populated
//	if (DropTable.Num() > 0)
//	{
//		// determine how many parts to drop
//		int32 NumDrops = FMath::RandRange(MinNumOfDrops, MaxNumOfDrops);
//
//		for (int i = 0; i < NumDrops; i++)
//		{
//			// get the item to drop
//			int32 DropIndex = FMath::RandRange(0, DropTable.Num() - 1);
//
//			ToSpawn = DropTable[DropIndex];
//
//			/*SpawnDrop();*/
//		}
//	}
//}

// Called when the monster takes damage
void AMonsterBase::DamageMonster_Implementation(float _damage, FVector _hitLoc, FName _boneHit)
{
	health -= _damage;
	
	/*bool isCrit = false;

	for each (FString Weakspot in WeakspotsScript)
	{
		if (Weakspot == _boneHit.ToString())
		{
			isCrit = true;
			break;
		}
	}*/
}

// Called when the monster successfully hits the player
void AMonsterBase::DamagePlayer_Implementation()
{
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

