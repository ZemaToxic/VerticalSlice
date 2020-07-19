// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GameFramework/PlayerController.h"
//#include "QuestPlugin.generated.h"

class FQuestPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	UFUNCTION(BlueprintCallable, Category = QuestPlugin)
	static void ProjectWorldToScreenQP(APlayerController const* Player, const FVector& WorldPosition, FVector2D& ScreenPosition);
};
