// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeMode/PurchasableDoors.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APurchasableDoors::APurchasableDoors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	DoorMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BoxComponent->SetBoxExtent(FVector(75.0f, 75.0f, 75.0f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APurchasableDoors::OnBoxBeginOverlap);

}

void APurchasableDoors::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//AInteractableVolume* interactItem = Interactable;
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (Interactable->GetActivated())
		{
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("OVERLAP REGISTERED")); }
		}
	}

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
}

  
