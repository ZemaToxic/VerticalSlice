// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MonsterBase.h"
#include "GM_Base.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float money;

	UFUNCTION(BlueprintCallable)
		void SellAllParts();
};
