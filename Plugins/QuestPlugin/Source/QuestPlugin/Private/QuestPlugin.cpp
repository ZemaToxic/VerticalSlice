// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuestPlugin.h"
#include "Engine/LocalPlayer.h"
#include "SceneView.h"

#define LOCTEXT_NAMESPACE "FQuestPluginModule"

void FQuestPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FQuestPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FQuestPluginModule::ProjectWorldToScreenQP(APlayerController const* Player, const FVector& WorldPosition, FVector2D& ScreenPosition)
{
	FVector Projected;
	bool bTargetBehindCamera = false;

	// Custom Projection Function
	ULocalPlayer* const LP = Player->GetLocalPlayer();
	if (LP && LP->ViewportClient)
	{
		FSceneViewProjectionData NewProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, EStereoscopicPass::eSSP_FULL, NewProjectionData))
		{
			const FMatrix ViewProjectionMatrix = NewProjectionData.ComputeViewProjectionMatrix();
			const FIntRect ViewRectangle = NewProjectionData.GetConstrainedViewRect();

			FPlane Result = ViewProjectionMatrix.TransformFVector4(FVector4(WorldPosition, 1.f));
			if (Result.W < 0.f) { bTargetBehindCamera = true; }
			if (Result.W == 0.f) { Result.W = 1.f; } // Prevent Divide By Zero

			const float RHW = 1.f / FMath::Abs(Result.W);
			Projected = FVector(Result.X, Result.Y, Result.Z) * RHW;

			// Normalize to 0..1 UI Space
			const float NormX = (Projected.X / 2.f) + 0.5f;
			const float NormY = 1.f - (Projected.Y / 2.f) - 0.5f;

			Projected.X = (float)ViewRectangle.Min.X + (NormX * (float)ViewRectangle.Width());
			Projected.Y = (float)ViewRectangle.Min.Y + (NormY * (float)ViewRectangle.Height());
		}
	}

	FVector2D ScreenPos = FVector2D(Projected.X, Projected.Y);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestPluginModule, QuestPlugin)