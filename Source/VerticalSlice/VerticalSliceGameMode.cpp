// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "VerticalSliceGameMode.h"
#include "VerticalSliceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVerticalSliceGameMode::AVerticalSliceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
