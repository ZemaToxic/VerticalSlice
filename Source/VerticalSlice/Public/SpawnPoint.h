// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestStruct.h"
#include "SpawnPoint.generated.h"

UCLASS()
class VERTICALSLICE_API ASpawnPoint : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Components")
		class USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		MonsterSpawnLocations SpawnPointLocation = MonsterSpawnLocations::TEST;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spawning")
		void SpawnNewActor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
