#include "C_WorldManager.h"

C_WorldManager::C_WorldManager()
	: uid(0xFFFF)
{
	m_activeEntities.reserve(32);
}

C_WorldManager::~C_WorldManager()
{
	clearEntities();
}

void C_WorldManager::clearEntities()
{
	for (auto iter = m_activeEntities.begin(); iter != m_activeEntities.end(); iter++)
		delete (*iter);
}

void C_WorldManager::removeEntity(const u16 uidToRemove)
{
	// Don't remove this player
	if (uidToRemove == uid)
		return;

	for (auto iter = m_activeEntities.begin(); iter != m_activeEntities.end(); iter++)
		if ((*iter)->uid == uidToRemove)
		{
			m_activeEntities.erase(iter);
			return;
		}
}

 std::vector<C_Entity*>& C_WorldManager::getActiveEntities()
 {
	return m_activeEntities;
}

C_Entity* C_WorldManager::getEntity(const u16 uid)
{
	assert(uid < MAX_ENTITIES);
	for (auto iter = m_activeEntities.begin(); iter != m_activeEntities.end(); iter++)
		if ((*iter)->uid == uid)
			return *iter;
	C_Entity* newEntity = new C_Entity(uid);
	m_activeEntities.push_back(newEntity);
	return newEntity;
}

C_Entity* C_WorldManager::getThisEntity()
{
	assert(uid == 0xFFFF || uid < MAX_ENTITIES);
	if (uid == 0xFFFF)
		return nullptr;
	else
		return getEntity(uid);
}

void C_WorldManager::setThisUID(const u16 playerUID)
{
	assert(playerUID < MAX_ENTITIES);
	this->uid = playerUID;
	auto playerEntity = getEntity(playerUID);
	playerEntity->setEntityType(EntityType::ET_PLAYER);
}

void C_WorldManager::update(const GameTime& gameTime)
{
	for (auto e = m_activeEntities.begin(); e != m_activeEntities.end();)
	{
		auto entity = (*e);
		entity->update(gameTime);
		if (entity->expired)
		{
			e = m_activeEntities.erase(e);
			continue;
		}
		e++;
	}
}
