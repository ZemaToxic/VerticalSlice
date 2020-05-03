// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

UCLASS()
class VERTICALSLICE_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		float maxHealth = 100;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "MonsterBase")
		void GetDamaged(float _damage);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
