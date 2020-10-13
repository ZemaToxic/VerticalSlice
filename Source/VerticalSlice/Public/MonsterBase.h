// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "DropsBase.h"
//#include "Perception/AIPerceptionComponent.h"
#include "MonsterBase.generated.h"

UENUM()
enum class MonsterDrops : uint8
{
	NOTHING,
	SHARPFANG,
	HARDENEDSPIKE,
	ROUGHHIDE,
};

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

	// isDead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
		bool isDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTreeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
		TArray<FString> WeakspotsScript;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
		MonsterDrops MonsterDrop = MonsterDrops::SHARPFANG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
		TSubclassOf <class ADropsBase> DropActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
		int32 MaxNumOfDrops = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
		int32 MinNumOfDrops = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
		float SpawnRadius = 300.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Monster")
		void DamagePlayer();

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Drops")
	//	void DropItems();

public:
	// overridden in BP to call the damage popup widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Monster")
		void DamageMonster(float _Damage, FVector _HitLoc, FName _BoneHit, float _KnockbackForce);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Monster")
		void StunMonster(float _Duration, FVector _LaunchVelocity);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Monster")
		void Knockback(AActor* _ActorRef, float _KnockbackForce);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
