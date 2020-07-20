// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MonsterBase.h"
#include "DropsBase.h"

#include "MonsterNestBase.generated.h"




///** Spawns and returns class T, respects default rotation and translation of root component. */
//template< class T >
//T* SpawnActor
//(
//	AActor* Owner = NULL,
//	APawn* Instigator = NULL,
//	bool bNoCollisionFail = false
//)
//{
//	return (T*)(GetWorld()->SpawnActor(T::StaticClass(), NAME_None, NULL, NULL, NULL, bNoCollisionFail, false, Owner, Instigator));
//}

UCLASS()
class VERTICALSLICE_API AMonsterNestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterNestBase();

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* NestMesh;

	UPROPERTY(EditAnywhere, Category = "Drops")
		TArray <TSubclassOf<ADropsBase>> DropTable;

	UPROPERTY(EditAnywhere, Category = "Drops")
		TSubclassOf<class ADropsBase> ToSpawn;

	UPROPERTY(EditAnywhere, Category = "Drops")
		int32 MaxNumOfDrops = 3;

	UPROPERTY(EditAnywhere, Category = "Drops")
		int32 MinNumOfDrops = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
		float SpawnRadius = 200.0f;

	/*UPROPERTY(EditAnywhere, Category = "Drops")
		TMap<AActor*, int32> DropMap;*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Drops")
		void DropItems();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Functions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Drops")
		void Destroy();

	UFUNCTION(BlueprintCallable)
		void SpawnDrop(FVector _spawnLoc);
};
