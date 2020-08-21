// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceBase.h"

#pragma region PART VALUES
#define VALUE_SHARPFANG 100
#define VALUE_HARDSPIKE 200
#define VALUE_ROUGHHIDE 300
#pragma endregion

/*
	Sets the data to get ready for transition into the next level

	@params TMap<MonsterDrops, int32>& _partsCollected, float _PlayerMoney, MechUpgrades _LastMechUpgrade, AbilityUpgrades _LastAbilityUpgrade, GunUpgrades _LastGunUpgrade
	@return void
	@author Jason Lu
*/
void UGameInstanceBase::SetData(TMap<MonsterDrops, int32> _partsCollected, float _PlayerMoney, TMap<FeatureUpgrades, bool> _GIFeatureUpgradesMap, TMap<StatUpgrades, int> _GIStatUpgradesMap)
{
	partsCollected = _partsCollected;
	PlayerMoney = _PlayerMoney;
	GIFeatureUpgradesMap = _GIFeatureUpgradesMap;
	GIStatUpgradesMap = _GIStatUpgradesMap;
}
