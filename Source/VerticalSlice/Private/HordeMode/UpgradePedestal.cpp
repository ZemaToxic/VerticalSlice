// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeMode/UpgradePedestal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUpgradePedestal::AUpgradePedestal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UpgradeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Upgrade Mesh"));
	UpgradeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->AttachToComponent(UpgradeMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	BoxComponent->SetBoxExtent(FVector(75.0f, 75.0f, 75.0f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUpgradePedestal::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AUpgradePedestal::OnBoxEndOverlap);

	fUpgradeCost = 250.0f;
}

// Called when the game starts or when spawned
void AUpgradePedestal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUpgradePedestal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Interactable->GetActivated())
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("INTERACT REGISTERED")); }
		CheckPurchase();
	}
}

void AUpgradePedestal::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PLAYER OVERLAP REGISTERED")); }
	}
}

void AUpgradePedestal::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PLAYER OVERLAP ENDED")); }
		// Reset the Interactable object
		Interactable->Reset();
	}
}

void AUpgradePedestal::CheckPurchase()
{
	AGM_HordeMode* const GameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
	if (GameMode)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("CORRECT GAMEMODE")); }
		float currentCash = GameMode->GetCurrency();
		if (currentCash >= fUpgradeCost)
		{
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("PLAYER HAS ENOUGH CASH")); }
		}
		else
		{
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("NOT ENOUGH MONEY")); }
		}
	}
}

void AUpgradePedestal::UpgradeMech()
{
}


