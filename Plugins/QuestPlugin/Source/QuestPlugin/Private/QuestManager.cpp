// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestManager.h"

#include "Engine.h"

AQuestManager::AQuestManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> OpenQuests(TEXT("DataTable'/QuestPlugin/Quests.Quests'"));
	if (OpenQuests.Succeeded())
	{
		Quests = OpenQuests.Object;
	}
}

AQuestManager::~AQuestManager()
{
}

void AQuestManager::LoadQuest(const FName QuestID, bool &success)
{
	if (NameToId.Contains(QuestID)) 
	{ 
		success = false;
		return; 
	}

	FString ContextString;
	FQuestStruct* ActiveQuestInit = Quests->FindRow<FQuestStruct>(QuestID, ContextString);
	
	success = (bool)ActiveQuestInit;

	if (success)
	{
		AActiveQuest* newQuest = GetWorld()->SpawnActor<AActiveQuest>();
		newQuest->initalise(QuestID, ActiveQuestInit, Quests);

		ActiveQuests.Add(newQuest);
		ActiveQuest = newQuest;

		NameToId.Add(QuestID, ActiveQuests.Num() - 1);
	}
}

void AQuestManager::UnloadQuest(int QuestId)
{
	if (QuestId < ActiveQuests.Num())
	{
		FName currentName = *(NameToId.FindKey(QuestId));
		NameToId.Remove(currentName);
		bool ActiveQuestBeingDeleted = (ActiveQuests[QuestId] == ActiveQuest);
		ActiveQuests[QuestId]->Destroy();
		ActiveQuests.RemoveAt(QuestId);

		for (int i = QuestId + 1; i < ActiveQuests.Num(); i++)
		{
			currentName = *(NameToId.FindKey(i));
			NameToId[currentName]--;
		}

		if (ActiveQuestBeingDeleted)
		{
			if (ActiveQuests.Num() > 0)
			{
				ActiveQuest = ActiveQuests[0];
			}
			else
			{
				ActiveQuest = NULL;
			}
		}
	}
}

void AQuestManager::UnloadQuest_Name(FName QuestId)
{
	if (NameToId.Contains(QuestId))
	{
		UnloadQuest(NameToId[QuestId]);
	}
}

void AQuestManager::setActiveQuest(int ActiveQuestId)
{
	if (ActiveQuestId < ActiveQuests.Num())
	{
		ActiveQuest = ActiveQuests[ActiveQuestId];
		ActiveQuestName = *(NameToId.FindKey(ActiveQuestId));
	}
}

void AQuestManager::setActiveQuest_Name(FName ActiveQuestId)
{
	if (NameToId.Contains(ActiveQuestId))
	{
		setActiveQuest(NameToId[ActiveQuestId]);
	}
}

void AQuestManager::ProjectWorldToScreenQP(APlayerController const* Player, const FVector& WorldPosition, FVector2D& ScreenPos) const
{
	FVector Projected;
	bool bTargetBehindCamera = false;

	// Custom Projection Function
	ULocalPlayer* const LP = Player ? Player->GetLocalPlayer() : nullptr;
	if (LP && LP->ViewportClient)
	{
		// get the projection data
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, eSSP_FULL, /*out*/ ProjectionData))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Yay");
			const FMatrix ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
			const FIntRect ViewRectangle = ProjectionData.GetConstrainedViewRect();

			FPlane Result = ViewProjectionMatrix.TransformFVector4(FVector4(WorldPosition, 1.f));
			if (Result.W < 0.f) { bTargetBehindCamera = true; }
			if (Result.W == 0.f) { Result.W = 1.f; } // Prevent Divide By Zero

			const float RHW = 1.f / FMath::Abs(Result.W);
			Projected = FVector(Result.X, Result.Y, Result.Z) * RHW;

			// Normalize to 0..1 UI Space
			float NormX = (Projected.X / 2.f) + 0.5f;
			float NormY = 1.f - (Projected.Y / 2.f) - 0.5f;

			if (bTargetBehindCamera)
			{
				NormX = FMath::RoundToFloat(NormX);
			}

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *Projected.ToString()));

			Projected.X = (float)ViewRectangle.Min.X + (NormX * (float)ViewRectangle.Width());
			Projected.Y = (float)ViewRectangle.Min.Y + (NormY * (float)ViewRectangle.Height());
		}
	}

	ScreenPos = FVector2D(Projected.X, Projected.Y);
}
