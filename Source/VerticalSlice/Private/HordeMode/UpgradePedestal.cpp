// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeMode/UpgradePedestal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AUpgradePedestal::AUpgradePedestal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UpgradeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Upgrade Mesh"));
	UpgradeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// Create Box collison component, To be used for Overlap Events.
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->AttachToComponent(UpgradeMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxComponent->SetBoxExtent(FVector(75.0f, 75.0f, 75.0f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUpgradePedestal::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AUpgradePedestal::OnBoxEndOverlap);
	// Create Text object and set Size and Scale.
	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Comp"));
	Text->SetTextRenderColor(FColor::Red);
	Text->SetText(FText::FromString(TEXT("Default Text")));
	Text->SetXScale(1.f);
	Text->SetYScale(1.f);
	Text->SetWorldSize(25);
	Text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Text->SetRelativeLocation(FVector(55.0, 0.0f, 0.0f));
	Text->SetVisibility(false);
	Text->bHiddenInGame = false;
	// Set defualt values just incase.
	fUpgradeCost = 250.0f;
	iCurrentUpgrade = 2;
}

// Called when the game starts or when spawned
void AUpgradePedestal::BeginPlay()
{
	Super::BeginPlay();
	// Create upgrade display Text array.
	PopulateUpgradeArray();
	// Set the current Upgrade.
	SetUpgrade();
}

// Called every frame
void AUpgradePedestal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Interactable->GetActivated())
	{
		CheckPurchase();
	}
}

void AUpgradePedestal::PopulateUpgradeArray()
{
	// Add all possible upgrade descriptions to the `Upgrades` array.
	Upgrades.Add("Boosters");
	Upgrades.Add("Shotgun");
	Upgrades.Add("Dash");
	Upgrades.Add("Ground Pound");
	Upgrades.Add("No Charge, Regen delay");
	Upgrades.Add("HP Potion");
	Upgrades.Add("Flamethrower");
	Upgrades.Add("Rocket Launcher");
	Upgrades.Add("Ammo Refill");
	Upgrades.Add("Health Refill");
	Upgrades.Add("Rifle Damage Upgrade");
	Upgrades.Add("Rifle Reload Upgrade");
	Upgrades.Add("Rifle Clip Upgrade");
	Upgrades.Add("Rifle Reserve Upgrade");
	Upgrades.Add("Shotgun Dammage Upgrade");
	Upgrades.Add("Shotgun Charges Upgrade");
	Upgrades.Add("Shotgun Pellets Upgrade");
	Upgrades.Add("Shotgun Range Upgrade");
	Upgrades.Add("Mech Max HP Upgrade");
	Upgrades.Add("Mech Max Charge Upgrade");
	Upgrades.Add("Mech HP Regen Upgrade");
	Upgrades.Add("Mech Charge Regen Upgrade");
	Upgrades.Add("Flamethrower Damage Upgrade");
	Upgrades.Add("Flamethrower Fire Damage Upgrade");
	Upgrades.Add("Rocket Amount Damage Upgrade");
	Upgrades.Add("Rocket Radius Fire Damage Upgrade");
}

void AUpgradePedestal::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping Actor is the Player Character.
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// Display the current Price of the Upgrade.
		Text->SetText(TEXT("$ " + FString::FromInt(fUpgradeCost) + " to unlock - " + Upgrades[iCurrentUpgrade]));
		Text->SetVisibility(true);
	}
}

void AUpgradePedestal::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the overlapping Actor is the Player Character.
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// Reset the Interactable object
		Interactable->Reset();
		Text->SetVisibility(false);
	}
}

void AUpgradePedestal::CheckPurchase()
{
	AGM_HordeMode* const GameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
	if (GameMode)
	{
		// Get the Players current money.
		float currentCash = GameMode->GetCurrency();
		// If the player has more money then the cost of the upgrade.
		if (currentCash >= fUpgradeCost)
		{
			// Upgrade the Mech with the current Upgrade then deduct cost from players total.
			UpgradeMech(iCurrentUpgrade);
			GameMode->SetCurrency(fUpgradeCost);
			// Remove Upgrade Mesh and hide text.
			UpgradeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UpgradeMesh->SetVisibility(false);
			Text->bHiddenInGame = true;
		}
	}
}

void AUpgradePedestal::UpgradeMech(int _iChoosenUpgade)
{
	AVerticalSliceCharacter* currentChar = Cast<AVerticalSliceCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (currentChar)
	{
		switch (_iChoosenUpgade)
		{
		case 1: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::Boosters, true);
			break;
		}
		case 2: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::Shotgun, true);
			break;
		}
		case 3: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::Dash, true);
			break;
		}
		case 4: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::GroundPound, true);
			break;
		}
		case 5: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::NoChargeRegenDelay, true);
			break;
		}
		case 6: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::HPRegen, true);
			break;
		}
		case 7: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::HPPotion, true);
			break;
		}
		case 8: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::Flamethrower, true);
			break;
		}
		case 9: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::RocketLauncher, true);
			break;
		}
		case 10: {
			currentChar->PlayerMech->giveAmmo(true);
			break;
		}
		case 11: {
			currentChar->PlayerMech->giveHealth(true);
			break;
		}
		case 12: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RifleDamage, 1, true);
			break;
		}
		case 13: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RifleReload, 1, true);
			break;
		}
		case 14: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RifleClipSize, 1, true);
			break;
		}
		case 15: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RifleReserveAmmo, 1, true);
			break;
		}
		case 16: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::ShotgunDamage, 1, true);
			break;
		}
		case 17: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::ShotgunCharges, 1, true);
			break;
		}
		case 18: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::ShotgunPellets, 1, true);
			break;
		}
		case 19: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::ShotgunRange, 1, true);
			break;
		}
		case 20: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::MechMaxHP, 1, true);
			break;
		}
		case 21: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::MechMaxCharge, 1, true);
			break;
		}
		case 22: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::MechHPRegen, 1, true);
			break;
		}
		case 23: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::MechChargeRegen, 1, true);
			break;
		}
		case 24: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::FlamethrowerDamage, 1, true);
			break;
		}
		case 25: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::FlamethrowerFireDamage, 1, true);
			break;
		}
		case 26: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RocketAmount, 1, true);
			break;
		}
		case 27: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RocketRadius, 1, true);
			break;
		}
		default:
			break;
		}
	}
}

void AUpgradePedestal::SetUpgrade()
{
	// Pick next Upgrade.
	iCurrentUpgrade = FMath::RandRange(1, Upgrades.Num());
	// Hide Upgrade "Box" and Text.
	UpgradeMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	UpgradeMesh->SetVisibility(true);
	Text->bHiddenInGame = false;
	// Set the price of the Upgrade.
	AGM_HordeMode* const GameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
	if (GameMode)
	{
		fUpgradeCost = GameMode->iCurrentRound * 250.0f;
	}
}