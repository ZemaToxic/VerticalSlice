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
	BoxComponent->AttachToComponent(UpgradeMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxComponent->SetBoxExtent(FVector(75.0f, 75.0f, 75.0f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUpgradePedestal::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AUpgradePedestal::OnBoxEndOverlap);

	fUpgradeCost = 250.0f;
	iCurrentUpgrade = 2;
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
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Current Upgrade: " + FString::FromInt(iCurrentUpgrade))); }
			UpgradeMech(iCurrentUpgrade);
		}
		else
		{
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("NOT ENOUGH MONEY")); }
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
		case 10:
		{
			currentChar->PlayerMech->giveAmmo(true);
			break;
		}
		case 11:
		{
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
		case 21:
		{
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
	iCurrentUpgrade = FMath::RandRange(1, 27);
}


