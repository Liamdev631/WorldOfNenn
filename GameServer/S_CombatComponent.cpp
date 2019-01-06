#include "S_CombatComponent.h"
#include "S_Entity.h"
#include "S_GlobalServer.h"
#include "Packets.h"

S_CombatComponent::S_CombatComponent(S_Entity& owner)
	: owner(owner), isInCombat(false), justDied(false), attackTimer(0), respawnTimer(1)
{
	combatState = CombatState(4);
	attackSpeed = 6;
	maxHitMelee = 4;
	respawnDelay = 16;
}

S_CombatComponent::~S_CombatComponent()
{

}

void S_CombatComponent::update()
{
	if (combatState.dead)
	{
		if (respawnTimer-- == 0)
		{
			combatState.reset();
			owner.onRespawn();
			owner.getMovement().forcePositionUpdate();
		}
	}

	if (attackTimer > 0)
		attackTimer--;
	if (attackTimer != 0)
		return;
	if (!isInCombat)
		return;

	if (inRange(*target))
	{
		// Attack
		attackTimer = attackSpeed;
		u8 damage = 0;
		if ((unsigned int)(rand()) % 1024 < 630)
			if (maxHitMelee == 1)
				damage = 1;
			else
				damage = 1 + rand() % (maxHitMelee - 1);

		if (damage > 0)
		{
			// Deal damage
			target->getCombat().takeDamage(*this, damage);

			// Give combat xp
			if (owner.getEntityType() == EntityType::ET_PLAYER)
			{
				auto thisPlayer = reinterpret_cast<S_Entity_Player*>(&owner);
				thisPlayer->addExperience(Skill::SK_Strength, (exp_val)max(1, damage / 2));
				thisPlayer->addExperience(Skill::SK_Constitution, (exp_val)max(1, damage / 4));
			}

			// Stop combat if dead
			if (target->getCombat().combatState.dead)
			{
				owner.getMovement().resetMovement();
				isInCombat = false;
			}
		}
	}
}

void S_CombatComponent::initiateCombat(S_Entity& newTarget)
{
	//if (!isMulticombat)
	if (isInCombat)
		return;
	//if (newTarget.getCombat().isInCombat)
	//	return;
	//}

	target = &newTarget;
	isInCombat = true;
	owner.getMovement().startFollow(*target);
}

void S_CombatComponent::takeDamage(S_CombatComponent& attacker, u8 damage)
{
	if (combatState.dead)
		return;

	// Retaliate
	//if (target->autoRetaliate)
	if (!isInCombat)
		initiateCombat(attacker.owner);

	auto et = owner.getEntityType();
	//if (et != ET_PLAYER && et != ET_ADMIN) // Temp make playesr immune to damage
	{
		if (damage >= combatState.currentHealth)
		{
			// Entity has no health left
			die();
		}
		else
		{
			// Take damage
			combatState.currentHealth -= damage;
		}
	}

	// Tell nearby players about the hit
	auto& connectionList = owner.getMovement().getWorldRegion().getConnections();
	for (auto& conn : connectionList)
	{
		auto& buffer = conn.second->getBuffer();
		buffer.write(SP_EntityTookDamage(owner.uid, damage, combatState));
	}
}

void S_CombatComponent::die()
{
	combatState.currentHealth = 0;
	combatState.dead = true;
	justDied = true;
	isInCombat = false;
	respawnTimer = respawnDelay;
	owner.getMovement().resetMovement();
	owner.onDeath();
}

u8 S_CombatComponent::getRange() const
{
	return owner.getEntityType() == ET_PLAYER ? 1 : 1;
}

bool S_CombatComponent::inRange(S_Entity& other) const
{
	return owner.getMovement().isWithinDistance(other.getMovement().getPos(), getRange());
}

void S_CombatComponent::reset()
{
	hits.clear();
	justDied = false;
}

void S_CombatComponent::totalReset()
{
	reset();
	combatState.reset();
}
