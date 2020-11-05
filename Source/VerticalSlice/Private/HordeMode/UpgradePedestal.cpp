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
	Text->SetWorldSize(20);
	Text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Text->SetRelativeLocation(FVector(55.0, 0.0f, 0.0f));
	Text->SetVisibility(false);
	Text->bHiddenInGame = false;
	// Set defualt values just incase.
	fUpgradeCost = 250.0f;
	fBaseCost = 250.0f;
	iCurrentUpgrade = 1;
}

// Called when the game starts or when spawned
void AUpgradePedestal::BeginPlay()
{
	Super::BeginPlay();
	// Create upgrade display Text array Set the current Upgrade.
	PopulateUpgradeArray();
	SetUpgrade();
}

// Called every frame
void AUpgradePedestal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Check if Interactable exists.
	if (Interactable)
	{
		// Check if the Interactable is Activated. Check if the upgrade has not been purchased already.
		if (Interactable->GetActivated() && !bSinglePurchase)
		{
			// Check if player can buy the upgrade.
			if (CanPurchase())
			{
				ConfirmPurchase();
			}
		} 
	}
}

/*
Description: Fill the Array of possible upgrades.
Author: Crystal Seymour
*/
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

/*
Description: Display the current cost to purchase the upgrade when the player overlaps.
Author: Crystal Seymour
*/
void AUpgradePedestal::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping Actor is the Player Character.
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// Display the current Price of the Upgrade.
		Text->SetText(TEXT("$ " + FString::FromInt(fUpgradeCost) + " to unlock - " + Upgrades[iCurrentUpgrade - 1]));
		Text->SetVisibility(true);
	}
}

/*
Description: Hide the text when the player leaves the pedestal.
Author: Crystal Seymour
*/
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

/*
Description: Check if the player has enough money to purchase the current upgrade.
Author: Crystal Seymour
*/
bool AUpgradePedestal::CanPurchase()
{
	if (GetWorld())
	{
		// Make sure upgrade can only be purchased once.
		bSinglePurchase = true;
		// Get the current GameMode.
		AGM_HordeMode* const GameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
		if (GameMode)
		{
			// Get the Players current money.
			float currentCash = GameMode->GetCurrency();
			// If the player has more money then the cost of the upgrade.
			if (currentCash >= fUpgradeCost)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return false;
}

/*
Description: Confirm the upgrade purchase then call UpgradeMech().
Author: Crystal Seymour
*/
void AUpgradePedestal::ConfirmPurchase()
{
	if (GetWorld())
	{
		AGM_HordeMode* const GameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
		if (GameMode)
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

/*
Description: Call the mech upgrade functions based on the current upgrade that is purchased.
Author: Crystal Seymour
*/
void AUpgradePedestal::UpgradeMech(int _iChoosenUpgade)
{
	AVerticalSliceCharacter* currentChar = Cast<AVerticalSliceCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MechUpgrades eChoosenUpgrade = (MechUpgrades)_iChoosenUpgade;
	
	if (currentChar)
	{
		switch (eChoosenUpgrade)
		{
		case Boosters: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::Boosters, true);
			break;
		}
		case Shotgun: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::Shotgun, true);
			break;
		}
		case Dash: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::Dash, true);
			break;
		}
		case GroundPound: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::GroundPound, true);
			break;
		}
		case NoChargeRegenDelay: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::NoChargeRegenDelay, true);
			break;
		}
		case HPRegen: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::HPRegen, true);
			break;
		}
		case HPPotion: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::HPPotion, true);
			break;
		}
		case Flamethrower: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::Flamethrower, true);
			break;
		}
		case RocketLauncher: {
			currentChar->PlayerMech->UpgradeFeatures(FeatureUpgrades::RocketLauncher, true);
			break;
		}
		case MaxAmmo: {
			currentChar->PlayerMech->giveAmmo(true);
			break;
		}
		case MaxHealth: {
			currentChar->PlayerMech->giveHealth(true);
			break;
		}
		case RifleDamage: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RifleDamage, 1, true);
			break;
		}
		case RifleReload: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RifleReload, 1, true);
			break;
		}
		case RifleClipSize: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RifleClipSize, 1, true);
			break;
		}
		case RifleReserveAmmo: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RifleReserveAmmo, 1, true);
			break;
		}
		case ShotgunDamage: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::ShotgunDamage, 1, true);
			break;
		}
		case ShotgunCharges: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::ShotgunCharges, 1, true);
			break;
		}
		case ShotgunPellets: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::ShotgunPellets, 1, true);
			break;
		}
		case ShotgunRange: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::ShotgunRange, 1, true);
			break;
		}
		case MechMaxHP: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::MechMaxHP, 1, true);
			break;
		}
		case MechMaxCharge: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::MechMaxCharge, 1, true);
			break;
		}
		case MechHPRegen: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::MechHPRegen, 1, true);
			break;
		}
		case MechChargeRegen: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::MechChargeRegen, 1, true);
			break;
		}
		case FlamethrowerDamage: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::FlamethrowerDamage, 1, true);
			break;
		}
		case FlamethrowerFireDamage: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::FlamethrowerFireDamage, 1, true);
			break;
		}
		case RocketAmount: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RocketAmount, 1, true);
			break;
		}
		case RocketRadius: {
			currentChar->PlayerMech->UpgradeStats(StatUpgrades::RocketRadius, 1, true);
			break;
		}
		default:
		// If I hit this, error out
			break;
		}
	}
}

/*
Description: Set the current upgrade to be purchased from the Pedestal when the player interacts with it.
Author: Crystal Seymour
*/
void AUpgradePedestal::SetUpgrade()
{
	// Pick next Upgrade.
	iCurrentUpgrade = FMath::RandRange(1, (Upgrades.Num() - 1));
	// Hide Upgrade "Box" and Text.
	UpgradeMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	UpgradeMesh->SetVisibility(true);
	Text->bHiddenInGame = false;
	// Set the price of the Upgrade.
	AGM_HordeMode* const GameMode = GetWorld()->GetAuthGameMode<AGM_HordeMode>();
	if (GameMode) { fUpgradeCost = GameMode->GetCurrentRound() * fBaseCost; }
	// Allow purchase of upgrade again.
	bSinglePurchase = false;
}