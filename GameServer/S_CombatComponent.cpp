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
			combatState.endUpdate();
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

	auto targetEntity = g_server->getWorldManager().getEntity(target);
	if (!targetEntity)
	{
		totalReset();
		return;
	}

	if (inRange(*targetEntity))
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
			targetEntity->getCombat().takeDamage(*this, damage);

			// Give combat xp
			if (owner.getEntityType() == EntityType::ET_PLAYER)
			{
				auto thisPlayer = reinterpret_cast<S_Entity_Player*>(&owner);
				thisPlayer->addExperience(Skill::SK_Strength, (exp_val)max(1, damage / 2));
				thisPlayer->addExperience(Skill::SK_Constitution, (exp_val)max(1, damage / 4));
			}

			// Stop combat if dead
			if (targetEntity->getCombat().combatState.dead)
			{
				owner.getMovement().resetMovement();
				isInCombat = false;
			}
		}
	}
}

void S_CombatComponent::initiateCombat(S_CombatComponent& newTarget)
{
	// Can't attack self
	if (newTarget.owner.uid == owner.uid)
	{
		auto player = owner.asPlayer();
		if (player)
			player->printInChatbox(L"You can't attack yourself, goofball!.");
		return;
	}

	// Can't attack something that is already in combat unless this is a multicombat zone
	if (newTarget.isInCombat)
	{
		auto player = owner.asPlayer();
		if (player)
			player->printInChatbox(L"That target is already in combat.");
		return;
	}

	target = newTarget.owner.uid;
	isInCombat = true;
	owner.getMovement().startFollow(newTarget.owner);
}

void S_CombatComponent::takeDamage(S_CombatComponent& attacker, u8 damage)
{
	if (combatState.dead)
		return;

	if (damage >= combatState.currentHealth)
	{
		// Entity has no health left
		die();
	}
	else
	{
		// Take damage
		combatState.currentHealth -= damage;
		if (!isInCombat)
			initiateCombat(attacker); // Retaliate
	}

	// Tell nearby players about the hit
	auto& connectionList = owner.getMovement().getWorldRegion().getPlayers();
	for (auto& conn : connectionList)
		conn.second->getBuffer().write(SP_EntityTookDamage(owner.uid, damage, combatState));
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

void S_CombatComponent::endUpdate()
{
	hits.clear();
	justDied = false;
}

void S_CombatComponent::totalReset()
{
	endUpdate();
	combatState.endUpdate();
}

bool S_CombatComponent::heal(u8 amount)
{
	if (combatState.currentHealth == combatState.maxHealth)
		return false;

	combatState.currentHealth += amount;
	if (combatState.currentHealth > combatState.maxHealth)
		combatState.currentHealth = combatState.maxHealth;

	// Notify nearby players
	auto region = owner.getMovement().getWorldRegion();
	for (auto player : region.getPlayers())
		player.second->getBuffer().write(SP_CombatStateChange(owner.uid, combatState));

	return true;
}

void S_CombatComponent::heal()
{
	combatState.currentHealth = combatState.maxHealth;
}
