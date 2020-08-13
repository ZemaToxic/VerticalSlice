// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "QuestStruct.h"
#include "GameFramework/Actor.h"
#include "ActiveQuest.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, NotPlaceable)
class QUESTPLUGIN_API AActiveQuest : public AActor
{
	GENERATED_BODY()

public:
	AActiveQuest();
	~AActiveQuest();

	void initalise(FName FirstQuestId, FQuestStruct* QuestInit, UDataTable* initQuestDataTable);

private: void initalise(FName FirstQuestId, FQuestStruct* QuestInit);

public:

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void getStage(int& currentStage) const { currentStage = stage; }

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void getStageName(FName& currentStageName) const { currentStageName = stageName; }

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void checkObjectiveValid(int index, bool& valid);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void checkAllObjectivesComplete(bool& complete);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void checkObjectiveComplete(int index, bool& complete);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void loadNextStage(bool& success);
	
	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void updateLocation(int objective, FVector newLoc);
	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void updateLocation_Name(FName  objective, FVector newLoc);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void updateAllLocations(FVector newLoc);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void updateValue(int objective, float newValue);
	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void updateValue_Name(FName  objective, float newValue);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void addToValue(int objective, float addedValue);
	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void addToValue_Name(FName  objective, float addedValue);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest")
	void getString(FString formatString, FString& outString, bool debug);

	FQuestStruct getObjectiveInfo(int index){ if (index < Quest.Num()) { return Quest[index]; } else { return FQuestStruct(); } }

	FVector getCurrentLoc(int index) { if (index < ObjectiveTracking.Num()) { return FVector(ObjectiveTracking[index]); } else { return FVector::ZeroVector; } }
	float getCurrentVal(int index) { if (index < ObjectiveTracking.Num()) { return ObjectiveTracking[index].W; } else { return 0.0f; } }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestPlugin | Quest", meta = (AllowPrivateAccess = true))
	TArray<FQuestStruct> Quest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestPlugin | Quest", meta = (AllowPrivateAccess = true))
	TArray<FVector4> ObjectiveTracking;

	UPROPERTY(BlueprintReadOnly, Category = "QuestPlugin | Quest", meta = (AllowPrivateAccess = true))
	bool currentObjectivesCompleted = false;

	UDataTable* QuestDataTable;
	TMap<FName, int> NameToId;

	int stage = 0;
	FName stageName;

};
