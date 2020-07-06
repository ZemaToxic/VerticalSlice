#pragma once
#include "CoreMinimal.h"

namespace Instructions
{
	enum ENums
	{
		CurrentLocationX,
		CurrentLocationY,
		CurrentLocationZ,
		CurrentValue,
		ObjectiveLocationX,
		ObjectiveLocationY,
		ObjectiveLocationZ,
		ObjectiveValue,
		QuestName,
		QuestDescription,
		ObjectiveName,
	};

	static TMap<FString, ENums> Map = 
	{
		{"clocx", ENums::CurrentLocationX},
		{"clocy", ENums::CurrentLocationY},
		{"clocz", ENums::CurrentLocationZ},
		{"cval", ENums::CurrentValue},
		{"olocx", ENums::ObjectiveLocationX},
		{"olocy", ENums::ObjectiveLocationY},
		{"olocz", ENums::ObjectiveLocationZ},
		{"oval", ENums::ObjectiveValue},
		{"qname", ENums::QuestName},
		{"qdesc", ENums::QuestDescription},
		{"oname", ENums::ObjectiveName},
	};
}