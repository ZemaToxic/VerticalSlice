// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
#include "Components/SkeletalMeshComponent.h"
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

// Called when the monster takes damage
void AMonsterBase::DamageMonster_Implementation(float _Damage, FVector _HitLoc, FName _BoneHit, float _KnockbackForce)
{
	health -= _Damage;
}

// Called when the monster successfully hits the player
void AMonsterBase::DamagePlayer_Implementation()
{
}

// Called when Monster should be stunned, primarily used for the Ground Pound Ability
void AMonsterBase::StunMonster_Implementation(float _Duration, FVector _LaunchVelocity)
{

}

// Called when the Monster should be knocked back, ActorRef is the actor the monster will be knocked away from
void AMonsterBase::Knockback_Implementation(AActor* _ActorRef, float _KnockbackForce)
{
}

void AMonsterBase::ActivateRagdoll_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
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

