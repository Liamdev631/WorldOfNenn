#include "S_Entity.h"
#include <assert.h>
#include "S_GlobalServer.h"

/// Constructor
S_Entity::S_Entity(const u16 uid, const EntityType entityType, const Region region)
	: uid(uid), m_entityType(entityType), m_combat(*this), m_movement(*this)
{
	m_movement.region = region;
}

/// Deconstructor
S_Entity::~S_Entity()
{

}

void S_Entity::update()
{

}

const EntityType& S_Entity::getEntityType() const
{
	return m_entityType;
}

S_CombatComponent& S_Entity::getCombat()
{
	return m_combat;
}

S_MovementComponent& S_Entity::getMovement()
{
	return m_movement;
}

void S_Entity::onRespawn()
{
	m_movement.blinkTo(m_movement.getPos());
}

void S_Entity::onDeath()
{
	// Drop bones
	auto& thisRegion = m_movement.getWorldRegion();
	thisRegion.addGroundItem(DropableItem(ItemStack(ITEM_BONES, 1), m_movement.getPos()));
}
