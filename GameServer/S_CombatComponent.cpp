#include "S_CombatComponent.h"
#include "S_Entity.h"
#include "S_GlobalServer.h"
#include "Packets.h"

S_CombatComponent::S_CombatComponent(S_Entity& owner)
	: m_owner(owner), isInCombat(false), justDied(false), attackTimer(0), respawnTimer(1)
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
			m_owner.onRespawn();
			m_owner.getMovement().forcePositionUpdate();
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

		// Deal damage
		target->getCombat().takeDamage(*this, damage);

		// Stop combat if dead
		if (target->getCombat().combatState.dead)
		{
			m_owner.getMovement().resetMovement();
			isInCombat = false;
		}
	}
}

void S_CombatComponent::initiateCombat(S_Entity& newTarget)
{
	//if (!isMulticombat)
	//	if (isInCombat)
	//		return;
	//	if (newTarget->getCombatComponent().isInCombat)
	//		return;
	//}

	target = &newTarget;
	isInCombat = true;
	m_owner.getMovement().startFollow(*target);
}

void S_CombatComponent::takeDamage(S_CombatComponent& attacker, u8 damage)
{
	if (combatState.dead)
		return;

	//if (target->autoRetaliate)
	if (!isInCombat)
		initiateCombat(attacker.m_owner);

	auto et = m_owner.getEntityType();
	//if (et != ET_PLAYER && et != ET_ADMIN) // Temp make playesr immune to damage
	{
		if (damage >= combatState.currentHealth)
		{
			// Entity has no health left
			combatState.currentHealth = 0;
			combatState.dead = true;
			justDied = true;
			respawnTimer = respawnDelay;

			isInCombat = false;
			attacker.isInCombat = false;

			m_owner.getMovement().resetMovement();
			target->getMovement().resetMovement();
			m_owner.onDeath();
		}
		else
			// Take damage
			combatState.currentHealth -= damage;
	}
	// Tell nearby players about the hit
	//auto playersList = g_server->getPlayers();
	auto& connectionList = m_owner.getMovement().getWorldRegion().getConnections();
	for (auto& conn : connectionList)
	{
		auto& buffer = conn.second->getBuffer();
		buffer.write(SP_EntityTookDamage(m_owner.getUID(), damage, combatState));
		//buffer.write<u8>(SP_);
		//buffer.write<u16>(m_owner.getUID());
		//buffer.write<u8>(damage);
		//buffer.write<CombatState>(combatState);
	}
}

u8 S_CombatComponent::getRange() const
{
	return m_owner.getEntityType() == ET_PLAYER ? 1 : 1;
}

bool S_CombatComponent::inRange(S_Entity& other) const
{
	return m_owner.getMovement().isWithinDistance(other.getMovement().getPos(), getRange());
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
