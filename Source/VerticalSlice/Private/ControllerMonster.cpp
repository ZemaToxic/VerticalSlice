// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerMonster.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MonsterBase.h"

AControllerMonster::AControllerMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	//// Sight config
	//SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	//// Add perception component
	//SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	//// Setup sight
	//SightConfig->SightRadius = AISightRadius;
	//SightConfig->LoseSightRadius = AILoseSightRadius;
	//SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	//SightConfig->SetMaxAge(AISightAge);

	//// Can detect all
	//SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	//SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	//SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


	//GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AControllerMonster::OnPawnDetected);
	//GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AControllerMonster::BeginPlay()
{
	Super::BeginPlay();

	// Get controlled pawn
	AMonsterBase* AIMonster = Cast<AMonsterBase>(GetPawn());

	UE_LOG(LogTemp, Warning, TEXT("Setup AI Controller"));

	// run behavior tree
	if (AIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting Behavior Tree"));
		RunBehaviorTree(AIMonster->BehaviorTreeComp);
	}

}

void AControllerMonster::OnPossess(APawn * _pawn)
{
	Super::OnPossess(_pawn);
}

void AControllerMonster::Tick(float _deltaSeconds)
{
	Super::Tick(_deltaSeconds);
}

//void AControllerMonster::OnPawnDetected(TArray<AActor*> DetectedPawns)
//{
//}


