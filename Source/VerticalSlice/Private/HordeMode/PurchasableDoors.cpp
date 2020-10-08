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
	DoorMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->AttachToComponent(DoorMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	BoxComponent->SetBoxExtent(FVector(75.0f, 75.0f, 75.0f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APurchasableDoors::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APurchasableDoors::OnBoxEndOverlap);
	

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Comp"));
	Text->SetTextRenderColor(FColor::Red);
	Text->SetText(FText::FromString(TEXT("Default Text")));
	Text->SetXScale(1.f);
	Text->SetYScale(1.f);
	Text->SetWorldSize(50);
	Text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Text->SetRelativeLocation(FVector(55.0, 0.0f, 0.0f));
	Text->SetVisibility(false);
	Text->bHiddenInGame = false;
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
	if (Interactable->GetActivated())
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("INTERACT REGISTERED")); }
		// Remove Door Mesh and allow play to walk through.
		DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DoorMesh->SetVisibility(false);
	}
}

void APurchasableDoors::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PLAYER OVERLAP REGISTERED")); }
		Text->SetText(TEXT("PLAYER OVERLAP"));
		Text->SetVisibility(true);

	}
}


void APurchasableDoors::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PLAYER OVERLAP ENDED")); }
		// Reset the Interactable object
		Interactable->Reset();
		Text->SetVisibility(false);

	}
}
  
