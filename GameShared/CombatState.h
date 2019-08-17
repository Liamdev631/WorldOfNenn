#pragma once
#include "Global.h"
#include "WPacket.h"
#include "RPacket.h"

#pragma pack(push, 1)
struct CombatState
{
	u8 maxHealth;
	u8 currentHealth;
	u8 poisoned : 1;
	u8 dead : 1;

	void reset()
	{
		currentHealth = maxHealth;
		poisoned = false;
		dead = false;
	}

	CombatState()
	{

	}

	CombatState(u8 maximumHealth)
	{
		maxHealth = maximumHealth;
		reset();
	}
};
#pragma pack(pop)
