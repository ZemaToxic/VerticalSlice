// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeMode/PurchasableDoors.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"

// Sets default values
APurchasableDoors::APurchasableDoors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	DoorMesh->AttachToComponent(Text, FAttachmentTransformRules::KeepRelativeTransform);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->AttachToComponent(Text, FAttachmentTransformRules::KeepRelativeTransform);
	BoxComponent->SetBoxExtent(FVector(75.0f, 75.0f, 75.0f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APurchasableDoors::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APurchasableDoors::OnBoxEndOverlap);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Comp"));
	Text->SetTextRenderColor(FColor::Red);
	Text->SetText(FText::FromString(TEXT("$500 to Unlock")));
	Text->SetXScale(1.f);
	Text->SetYScale(1.f);
	Text->SetWorldSize(25);
	Text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Text->SetRelativeLocation(FVector(55.0, 0.0f, 0.0f));
	Text->SetVisibility(false);
	Text->bHiddenInGame = false;

	fDoorCost = 500.0f;
}

// Called when the game starts or when spawned
void APurchasableDoors::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APurchasableDoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Interactable)
	{
		if (Interactable->GetActivated())
		{
			CanPurchase();
		}
	}
}

/*
Description: Set the door unlock cost.
Author: Crystal Seymour
*/
void APurchasableDoors::SetDoorCost(float _newCost)
{
	fDoorCost = _newCost;
}

/*
Description: When the player overlaps display the cost of the door.
Author: Crystal Seymour
*/
void APurchasableDoors::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		Text->SetWorldSize(25);
		Text->SetText(TEXT("$" + FString::FromInt(fDoorCost) + " to unlock."));
		Text->SetVisibility(true);
	}
}

/*
Description: Hide the door cost text when the player leaves the area.
Author: Crystal Seymour
*/
void APurchasableDoors::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// Reset the Interactable object
		Interactable->Reset();
		Text->SetVisibility(false);
	}
}

/*
Description: Check if the player has enough money to unlock the door.
Author: Crystal Seymour
*/
void APurchasableDoors::CanPurchase()
{
	if (GetWorld())
	{
		// Get the current GameMode.
		AGM_HordeMode* const GameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
		if (GameMode)
		{
			// Get the Players current money.
			float currentCash = GameMode->GetCurrency();
			// If the player has more money then the cost of the upgrade.
			if (currentCash >= fDoorCost)
			{
				ConfirmPurchase();
			}
			else
			{
				Text->SetWorldSize(15);
				Text->SetText(TEXT("You do not have enough Cash to unlock this."));
			}
		}
	}
}

/*
Description: Unlock the door then remove it from the level so that the player can walk through.
Author: Crystal Seymour
*/
void APurchasableDoors::ConfirmPurchase()
{
	if (GetWorld())
	{
		AGM_HordeMode* const GameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
		if (GameMode)
		{
			// Deduct cost of door from players money.
			GameMode->SetCurrency(fDoorCost);
			// Remove Door Mesh and allow play to walk through.
			DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DoorMesh->SetVisibility(false);
			Text->bHiddenInGame = true;
			Interactable->Destroy();
			Destroy();
		}
	}
}
