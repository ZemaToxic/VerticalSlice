// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceBase.h"

#pragma region PART VALUES
#define VALUE_SHARPFANG 100
#define VALUE_HARDSPIKE 200
#define VALUE_ROUGHHIDE 300
#pragma endregion

///*
//	Sells all the parts the player has collected and converts it to money
//
//	@params void
//	@return void
//	@author Jason Lu
//*/
//void UGameInstanceBase::SellAllParts()
//{
//	for (auto& Part : partsCollected)
//	{
//		switch (Part.Key)
//		{
//		case MonsterDrops::SHARPFANG:
//			PlayerMoney += VALUE_SHARPFANG * Part.Value;
//			break;
//		case MonsterDrops::HARDENEDSPIKE:
//			PlayerMoney += VALUE_HARDSPIKE * Part.Value;
//			break;
//		case MonsterDrops::ROUGHHIDE:
//			PlayerMoney += VALUE_ROUGHHIDE * Part.Value;
//			break;
//		default:
//			break;
//		}
//		Part.Value = 0;
//	}
//}
//
///*
//	Adds Monster Parts to the PartsCollected map
//
//	@params MonsterDrops _dropType, int _amount
//	@return void
//	@author Jason Lu
//*/
//void UGameInstanceBase::AddParts(MonsterDrops _dropType, int _amount)
//{
//	for (auto& Part : partsCollected)
//	{
//		if (_dropType == Part.Key)
//		{
//			Part.Value += _amount;
//		}
//	}
//}

//void UGameInstanceBase::SetData_Implementation(TMap<MonsterDrops, int32> _partsCollected, float _PlayerMoney, MechUpgrades _LastMechUpgrade, AbilityUpgrades _LastAbilityUpgrade, GunUpgrades _LastGunUpgrade)
//{
//	partsCollected = _partsCollected;
//	PlayerMoney = _PlayerMoney;
//	LastMechUpgrade = _LastMechUpgrade;
//	LastAbilityUpgrade = _LastAbilityUpgrade;
//	LastGunUpgrade = _LastGunUpgrade;
//}

void UGameInstanceBase::SetData(TMap<MonsterDrops, int32>& _partsCollected, float _PlayerMoney, MechUpgrades _LastMechUpgrade, AbilityUpgrades _LastAbilityUpgrade, GunUpgrades _LastGunUpgrade)
{
	partsCollected = _partsCollected;
	PlayerMoney = _PlayerMoney;
	LastMechUpgrade = _LastMechUpgrade;
	LastAbilityUpgrade = _LastAbilityUpgrade;
	LastGunUpgrade = _LastGunUpgrade;
}
