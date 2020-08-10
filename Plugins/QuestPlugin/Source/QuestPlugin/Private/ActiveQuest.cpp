// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveQuest.h"

#include "FormatInstructions.h"

#include "Misc/DefaultValueHelper.h"
#include "Engine.h"

AActiveQuest::AActiveQuest()
{
}

AActiveQuest::~AActiveQuest()
{
}

void AActiveQuest::initalise(FName FirstQuestId, FQuestStruct* QuestInit, UDataTable* initQuestDataTable)
{
	QuestDataTable = initQuestDataTable;
	
	initalise(FirstQuestId, QuestInit);
}

void AActiveQuest::initalise(FName FirstQuestId, FQuestStruct* QuestInit)
{
	stage++;
	stageName = FirstQuestId;
	currentObjectivesCompleted = false;
	Quest.Empty();
	ObjectiveTracking.Empty();
	NameToId.Empty();

	FName QuestId = FirstQuestId;
	int ObjectiveId = 0;
	FString ContextString;
	FQuestStruct* ObjectiveInit;
	bool success = true;
	FQuestStruct Objective = *QuestInit;
	FVector4 tempVect = FVector4();

	Quest.Add(Objective);
	ObjectiveTracking.Add(tempVect);

	NameToId.Add(QuestId, ObjectiveId);
	ObjectiveId++;


	while (Objective.HasMoreObjectives && success)
	{
		QuestId = Objective.NextObjectiveName;
		ObjectiveInit = QuestDataTable->FindRow<FQuestStruct>(QuestId, ContextString);

		success = (bool)ObjectiveInit;

		if (success)
		{
			Objective = *ObjectiveInit;
			Quest.Add(Objective);
			ObjectiveTracking.Add(tempVect);
			NameToId.Add(QuestId, ObjectiveId);
			ObjectiveId++;
		}
	}
}

void AActiveQuest::checkObjectiveValid(int index, bool& valid)
{
	valid = index < Quest.Num();
}

void AActiveQuest::checkAllObjectivesComplete(bool& complete)
{
	FQuestStruct Objective;
	FVector4 TrackedValues;
	bool AtLoc, AtValue;

	for (int i = 0; i < Quest.Num(); i++)
	{
		Objective = Quest[i];
		TrackedValues = ObjectiveTracking[i];
		AtLoc = true;
		AtValue = true;
		if (Objective.UsesLocation)
		{
			AtLoc = Objective.Location.Equals(FVector(TrackedValues), Objective.LocationActivationRadius);
		}
		if (Objective.UsesValue)
		{
			AtValue = TrackedValues.W > Objective.ObjectiveCompleteValue;
		}
		//UE_LOG(LogTemp, Warning, TEXT("%i"), Quest.Num())
		if (!AtLoc || !AtValue)
		{
			currentObjectivesCompleted = false;
			complete = currentObjectivesCompleted;
			return;
		}
	}

	currentObjectivesCompleted = true;
	complete = currentObjectivesCompleted;
}

void AActiveQuest::checkObjectiveComplete(int index, bool& complete)
{
	if (index >= Quest.Num()) { complete = true; return; }

	FQuestStruct Objective = Quest[index];
	FVector4 TrackedValues = ObjectiveTracking[index];
	bool AtLoc = true, AtValue = true;
	if (Objective.UsesLocation)
	{
		AtLoc = Objective.Location.Equals(FVector(TrackedValues), Objective.LocationActivationRadius);
	}
	if (Objective.UsesValue)
	{
		AtValue = TrackedValues.W > Objective.ObjectiveCompleteValue;
	}

	complete = AtLoc && AtValue;
}

void AActiveQuest::loadNextStage(bool& success)
{
	FQuestStruct LastObjective = Quest[Quest.Num() - 1];
	if (!LastObjective.HasMoreObjectives)
	{
		success = false;
		return;
	}

	FString ContextString;
	FName nextQuestId = LastObjective.NextQuestId;
	FQuestStruct* nextQuest = QuestDataTable->FindRow<FQuestStruct>(nextQuestId, ContextString);

	if (nextQuest)
	{
		initalise(nextQuestId, nextQuest);
		success = true;
		return;
	}

	success = false;
}

void AActiveQuest::updateLocation(int objective, FVector newLoc)
{
	if (objective < ObjectiveTracking.Num())
	{
		ObjectiveTracking[objective] = FVector4(newLoc, ObjectiveTracking[objective].W);
	}
}

void AActiveQuest::updateLocation_Name(FName objective, FVector newLoc)
{
	if (NameToId.Contains(objective))
	{
		updateLocation(NameToId[objective], newLoc);
	}
}

void AActiveQuest::updateAllLocations(FVector newLoc)
{
	for (int i = 0; i<Quest.Num(); i++)
	{
		if (Quest[i].UsesLocation)
		{
			ObjectiveTracking[i] = FVector4(newLoc, ObjectiveTracking[i].W);
		}
	}
}

void AActiveQuest::updateValue(int objective, float newValue)
{
	if (objective < ObjectiveTracking.Num())
	{
		ObjectiveTracking[objective].W = newValue;
	}
}

void AActiveQuest::updateValue_Name(FName objective, float newValue)
{
	if (NameToId.Contains(objective))
	{
		updateValue(NameToId[objective], newValue);
	}
}

void AActiveQuest::addToValue(int objective, float addedValue)
{
	if (objective < ObjectiveTracking.Num())
	{
		ObjectiveTracking[objective].W += addedValue;
	}
}

void AActiveQuest::addToValue_Name(FName  objective, float addedValue)
{
	if (NameToId.Contains(objective))
	{
		addToValue(NameToId[objective], addedValue);
	}
}

void AActiveQuest::getString(FString formatString, FString& outString, bool debug)
{
	//"c, v, m, l, n, d, o"

	//UE_LOG(LogTemp, Warning, TEXT("in getString"));

	outString.Empty();
	formatString = formatString.ToLower();
	FString currentString = "";
	FString currentInstruction = "";
	int currentObjective = 0;
	for (TCHAR currentchar : formatString)
	{
		if (currentchar == ' ') 
		{ 
			if (!currentString.StartsWith("%") || debug)
			{
				outString += currentString + currentchar;
			}

			currentString.Empty(); 
			currentInstruction.Empty();
			continue; 
		}

		if (currentchar == '\n')
		{
			if (!currentString.StartsWith("%") || debug)
			{
				outString += currentString + currentchar;
			}

			currentString.Empty();
			currentInstruction.Empty();
			continue;
		}

		currentString += currentchar;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, currentString);

		if (!currentString.StartsWith("%"))
		{
			outString += currentString;
			currentString.Empty();
			currentInstruction.Empty();
			continue;
		}

		currentInstruction = currentString;
		currentInstruction.RemoveAt(0);
		currentInstruction = currentInstruction.ToLower();

		if (FDefaultValueHelper::ParseInt(currentInstruction, currentObjective))
		{ 
			if (currentObjective < ObjectiveTracking.Num())
			{
				currentString.Empty();
				currentInstruction.Empty();
				continue;
			}
			else
			{
				currentObjective = 0;
				currentString.Empty();
				currentInstruction.Empty();
				break;
			}
		}

		if (!(Instructions::Map.Contains(currentInstruction))) { continue; }

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, currentInstruction);

		switch (Instructions::Map[currentInstruction])
		{
		case Instructions::ENums::CurrentLocationX:
		{
			if (!Quest[currentObjective].UsesLocation) { break; }
			outString.AppendInt(ObjectiveTracking[currentObjective].X);
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::CurrentLocationY:
		{
			if (!Quest[currentObjective].UsesLocation) { break; }
			outString.AppendInt(ObjectiveTracking[currentObjective].Y);
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::CurrentLocationZ:
		{
			if (!Quest[currentObjective].UsesLocation) { break; }
			outString.AppendInt(ObjectiveTracking[currentObjective].Z);
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::CurrentValue:
		{
			if (!Quest[currentObjective].UsesValue) { break; }
			//UE_LOG(LogTemp, Warning, TEXT("%i"), ObjectiveTracking[currentObjective].W - 1);
			outString.AppendInt(ObjectiveTracking[currentObjective].W - 1);
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::ObjectiveLocationX:
		{
			if (!Quest[currentObjective].UsesLocation) { break; }
			outString.AppendInt(Quest[currentObjective].Location.X);
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::ObjectiveLocationY:
		{
			if (!Quest[currentObjective].UsesLocation) { break; }
			outString.AppendInt(Quest[currentObjective].Location.Y);
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::ObjectiveLocationZ:
		{
			if (!Quest[currentObjective].UsesLocation) { break; }
			outString.AppendInt(Quest[currentObjective].Location.Z);
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::ObjectiveName:
		{
			outString += Quest[currentObjective].ObjectiveName;
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::ObjectiveValue:
		{
			if (!Quest[currentObjective].UsesValue) { break; }
			outString.AppendInt(Quest[currentObjective].ObjectiveCompleteValue);
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::QuestDescription:
		{
			outString += Quest[currentObjective].QuestDescription;
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		case Instructions::ENums::QuestName:
		{
			outString += Quest[currentObjective].QuestName;
			currentString.Empty();
			currentInstruction.Empty();
			break;
		}
		default:
			break;
		}
	}
}
