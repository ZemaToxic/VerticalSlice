// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
//#include "AIControllerBase.h"

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
void AMonsterBase::DamageMonster(float _damage)
{
	health -= _damage;
}

// Called when the monster successfully hits the player
void AMonsterBase::DamagePlayer()
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

