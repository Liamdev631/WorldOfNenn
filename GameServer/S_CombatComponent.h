#pragma once
#include "Global.h"
#include <vector>
#include "CombatState.h"

class S_Entity;

class S_CombatComponent
{
private:
	S_Entity& owner;

public:
	u16 target;
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

	// Returns the range at which this entity can attack another entity.
	u8 getRange() const;

	// Tick functinos
	void update();
	void endUpdate();
	void totalReset();

	// Utiltiy functions
	bool inRange(S_Entity& other) const;

	// Damage functions

	// Attempts to initiate combat with the given entity.
	// @target: The combat component of the entity to attack
	void initiateCombat(S_CombatComponent& target);

	// Attemps to remove health from this entity.
	// @attacker: The person dealing the damage.
	// @damage: The amount of damage to take. 
	void takeDamage(S_CombatComponent& attacker, u8 damage);

	// Kills this entity, removing all health and notifying all players
	void die();

	// Heals the entity for up to the given amount of health.
	// @returns: true if the entity was healed for any amount. Useful for determining if a healing item was consumed or not.
	bool heal(u8 amount);
	
	// Returns the entities current health to the maximum health.
	void heal();
};
