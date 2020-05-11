// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Perception/AIPerceptionComponent.h"
#include "MonsterBase.generated.h"

UCLASS()
class VERTICALSLICE_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
		float maxHealth = 100.f;

	// the amount of damage the monster will deal to the player when it hits it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
		float damage = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTreeComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Monster")
		void DamageMonster(float _damage);

	UFUNCTION(BlueprintCallable, Category = "Monster")
		void DamagePlayer();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
