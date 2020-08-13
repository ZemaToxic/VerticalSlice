// Fill out your copyright notice in the Description page of Project Settings.

//#include "MonsterBase.h"
#include "GM_Base.h"

#pragma region PART VALUES
#define VALUE_SHARPFANG 100
#define VALUE_HARDSPIKE 200
#define VALUE_ROUGHHIDE 300
#pragma endregion


/*
	Sells all the parts the player has collected and converts it to money

	@params void
	@return void
	@author Jason Lu
*/
void AGM_Base::SellAllParts()
{
	for (auto& Part : partsCollected)
	{
		switch (Part.Key)
		{
		case MonsterDrops::SHARPFANG:
			money += VALUE_SHARPFANG * Part.Value;
			break;
		case MonsterDrops::HARDENEDSPIKE:
			money += VALUE_HARDSPIKE * Part.Value;
			break;
		case MonsterDrops::ROUGHHIDE:
			money += VALUE_ROUGHHIDE * Part.Value;
			break;
		default:
			break;
		}
		Part.Value = 0;
	}
}

/*
	Returns true if the player has currently collected no parts

	@params void
	@return bool
	@author Jason Lu
*/
bool AGM_Base::CheckIfPartsEmpty()
{
	for (auto& Part : partsCollected)
	{
		if (Part.Value == 0) break;
		else return false;
	}
	return true;
}

/*
	This function creates a new FSpawnInfo object and adds it to the SpawnQueue to be spawned by the SpawnMonsters() function

	@params MonsterSpawnLocations _loc, int _numbersToSpawn, TSubclassOf<APawn> _monster, UBehaviorTree* _BT
	@return void
	@author Jason Lu
*/
void AGM_Base::SpawnMonstersAtLocation_Implementation(MonsterSpawnLocations _loc, int _numbersToSpawn, TSubclassOf<APawn> _monster, UBehaviorTree* _BT)
{
	if (_numbersToSpawn <= 0) return;

	FSpawnInfo NewSpawnInfo = FSpawnInfo();
	NewSpawnInfo.LocationToSpawn = _loc;
	NewSpawnInfo.MonsterToSpawn = _monster;
	NewSpawnInfo.BT = _BT;

	for (int i = 0; i < _numbersToSpawn; i++)
	{
		SpawnQueue.Add(NewSpawnInfo);
	}

	SpawnMonsters();
}

/*
	Adds Monster Parts to the PartsCollected map

	@params MonsterDrops _dropType, int _amount
	@return void
	@author Jason Lu
*/
void AGM_Base::AddParts_Implementation(MonsterDrops _dropType, int _amount)
{
	for (auto& Part : partsCollected)
	{
		if (_dropType == Part.Key)
		{
			Part.Value += _amount;
		}
	}
}
