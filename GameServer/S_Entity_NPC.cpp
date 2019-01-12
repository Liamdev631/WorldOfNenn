#include "S_Entity_NPC.h"
#include "S_GlobalServer.h"
#include "LootTable.h"

S_Entity_NPC::S_Entity_NPC(const u16 uid, const EntityType entityType, const Region region)
	: S_Entity(uid, entityType, region), m_stepTimer(4)
{
	m_combat.maxHitMelee = 1;
	m_combat.attackSpeed = 10;
}

S_Entity_NPC::~S_Entity_NPC()
{

}

void S_Entity_NPC::update()
{
	if (m_movement.getMoveState() == MoveState::Idle)
		m_stepTimer--;
	if (m_stepTimer == 0)
	{
		m_stepTimer = 8 + rand() % 16;
		vec2<u16> dest = m_boundsLocation;
		dest.x += rand() % m_boundsSize.x;
		dest.y += rand() % m_boundsSize.y;
		m_movement.moveToPosition(dest);
	}
}

void S_Entity_NPC::onRespawn()
{
	vec2s spawnPoint = m_boundsLocation;
	spawnPoint.x += rand() % m_boundsSize.x;
	spawnPoint.y += rand() % m_boundsSize.y;
	m_movement.blinkTo(spawnPoint);
}

void S_Entity_NPC::onDeath()
{
	S_Entity::onDeath();

	// Drop from the loot table
	auto& region = m_movement.getWorldRegion();
	auto loot = LootGenerator::generateLoot(entityType);
	for (auto& entry : loot)
		region.addGroundItem(DropableItem(entry, m_movement.getPos()));
}

void S_Entity_NPC::setBounds(const vec2<u16>& position, const vec2<u8>& size)
{
	m_boundsLocation = position;
	m_boundsSize = size;

	vec2s randomPos = m_boundsLocation;
	randomPos.x += rand() % m_boundsSize.x;
	randomPos.y += rand() % m_boundsSize.y;
}
