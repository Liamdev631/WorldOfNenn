#include "C_WorldManager.h"

C_WorldManager::C_WorldManager()
	: m_uid(0xFFFF)
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

void C_WorldManager::removeEntity(const u16 uid)
{
	for (auto iter = m_activeEntities.begin(); iter != m_activeEntities.end(); iter++)
		if ((*iter)->uid == uid)
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
	assert(m_uid == 0xFFFF || m_uid < MAX_ENTITIES);
	if (m_uid == 0xFFFF)
		return nullptr;
	else
		return getEntity(m_uid);
}

void C_WorldManager::setThisUID(const u16 uid)
{
	assert(uid < MAX_ENTITIES);
	m_uid = uid;
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
