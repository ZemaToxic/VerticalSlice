// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "QuestStruct.h"
#include "ActiveQuest.h"

#include "QuestManager.generated.h"

/**
 *
 */
UCLASS()
class QUESTPLUGIN_API AQuestManager : public AActor
{
	GENERATED_BODY()

private:
	UDataTable* Quests;
	TMap<FName, int> NameToId;

public:

	UPROPERTY(BlueprintReadOnly, Category = QuestData, meta = (AllowPrivateAccess = "true"))
		TArray<AActiveQuest*> ActiveQuests;

	UPROPERTY(BlueprintReadOnly, Category = QuestData, meta = (AllowPrivateAccess = "true"))
		AActiveQuest* ActiveQuest;

	UPROPERTY(BlueprintReadOnly, Category = QuestData, meta = (AllowPrivateAccess = "true"))
		FName ActiveQuestName;

	AQuestManager();
	~AQuestManager();

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest Manager")
	void LoadQuest(FName QuestID, bool& success);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest Manager")
	void UnloadQuest(int QuestId);
	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest Manager")
	void UnloadQuest_Name(FName QuestId);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest Manager")
	void setActiveQuest(int ActiveQuestId);
	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Quest Manager")
	void setActiveQuest_Name(FName ActiveQuestId);

	UFUNCTION(BlueprintCallable, Category = "QuestPlugin | Tools")
		void ProjectWorldToScreenQP(APlayerController const* Player, const FVector& WorldPosition, FVector2D& ScreenPos) const;
};