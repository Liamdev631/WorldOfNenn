#pragma once
#include "Global.h"
#include <vector>
#include "CombatState.h"

class S_Entity;

class S_CombatComponent
{
private:
	S_Entity& m_owner;

public:
	S_Entity* target;
	CombatState combatState;
	std::vector<u8> hits;
	bool isInCombat;
	bool justDied;
	u8 attackTimer;
	u8 attackSpeed;
	u8 respawnDelay;
	u8 respawnTimer;

	// Melee
	u8 maxHitMelee;

public:
	S_CombatComponent(S_Entity& owner);
	~S_CombatComponent();

	u8 getRange() const;

	bool inRange(S_Entity& other) const;
	void attackTarget(S_Entity& target);
	void reset();
	void totalReset();
	void tick();
	void takeDamage(S_CombatComponent& attacker, u8 damage);
};
