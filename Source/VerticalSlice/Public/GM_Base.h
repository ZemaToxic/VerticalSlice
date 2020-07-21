// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MonsterBase.h"
#include "SpawnPoint.h"
#include "GM_Base.generated.h"

USTRUCT(BlueprintType)
struct FSpawnInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		MonsterSpawnLocations LocationToSpawn = MonsterSpawnLocations::WATERPIPE1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TSubclassOf<APawn> MonsterToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		UBehaviorTree* BT;
};

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API AGM_Base : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TMap<MonsterDrops, int32> partsCollected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		MonsterSpawnLocations CurrentSpawnLocation = MonsterSpawnLocations::WATERPIPE1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TSubclassOf<APawn> MonsterToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		UBehaviorTree* BT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TArray<FSpawnInfo> SpawnQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float money;

	UFUNCTION(BlueprintCallable)
		void SellAllParts();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void AddParts(MonsterDrops _dropType, int _amount);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Spawning")
		void SpawnMonsters();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spawning")
		void SpawnMonstersAtLocation(MonsterSpawnLocations _loc, int _numbersToSpawn, TSubclassOf<APawn> _monster, UBehaviorTree* _BT);
};
