// Fill out your copyright notice in the Description page of Project Settings.

//#include "MonsterBase.h"
#include "GM_Base.h"

#pragma region PART VALUES
#define VALUE_SHARPFANG 100
#define VALUE_HARDSPIKE 200
#define VALUE_ROUGHHIDE 300
#pragma endregion



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

void AGM_Base::SpawnMonstersAtLocation_Implementation(MonsterSpawnLocations _loc, int _numbersToSpawn, TSubclassOf<APawn> _monster, UBehaviorTree* _BT)
{
	CurrentSpawnLocation = _loc;
	MonsterToSpawn = _monster;
	BT = _BT;

	for (int i = 0; i < _numbersToSpawn; i++)
	{
		SpawnMonsters();
	}
}

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
