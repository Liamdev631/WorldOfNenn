#include "S_Entity_NPC.h"
#include "S_GlobalServer.h"
#include "LootTable.h"

S_Entity_NPC::S_Entity_NPC(const u16 uid, const EntityType entityType, const Region region)
	: S_Entity(uid, entityType, region), m_stepTimer(4)
{
	m_movement.moveKey.pos = vec2s(40 + rand() % 30, 40 + rand() % 20);
	boundsLocation = m_movement.moveKey.pos;
	boundsSize = vec2<u8>(3 + rand() % 3, 3 + rand() % 3);

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
		vec2<u16> dest = boundsLocation;
		dest.x += rand() % boundsSize.x;
		dest.y += rand() % boundsSize.y;
		m_movement.moveToPosition(dest);
	}
}

void S_Entity_NPC::onRespawn()
{
	vec2s spawnPoint = boundsLocation;
	spawnPoint.x += rand() % boundsSize.x;
	spawnPoint.y += rand() % boundsSize.y;
	m_movement.blinkTo(spawnPoint);
}

void S_Entity_NPC::onDeath()
{
	S_Entity::onDeath();

	// Drop from the loot table
	auto& region = m_movement.getWorldRegion();
	auto loot = LootGenerator::generateLoot(EntityType::ET_BIGRAT);
	for (auto& entry : loot)
		region.addGroundItem(DropableItem(entry, m_movement.getPos()));
}
