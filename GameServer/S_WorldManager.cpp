#include "S_WorldManager.h"
#include "S_Connection.h"
#include <assert.h>

S_WorldManager::S_WorldManager()
{
	m_entities = new S_Entity*[MAX_ENTITIES];
	for (int i = 0; i < MAX_ENTITIES; i++)
		m_entities[i] = nullptr;
	m_entitiesList.reserve(MAX_ENTITIES);

	// Initialize regions
	m_regions = new S_Region*[NUM_REGIONS];
	for (size_t i = 0; i < NUM_REGIONS; i++)
		m_regions[i] = new S_Region(*this);
}

S_WorldManager::~S_WorldManager()
{
	for (size_t i = 0; i < NUM_REGIONS; i++)
		delete m_regions[i];
	delete m_regions;
}

#pragma region Getter / Setter

S_Entity* S_WorldManager::getEntity(u16 index) const
{
	return m_entities[index];
}

std::vector<S_Entity*>& S_WorldManager::getEntities()
{
	return m_entitiesList;
}

S_Region& S_WorldManager::getRegion(const Region region) const
{
	return *m_regions[region];
}

#pragma endregion

void S_WorldManager::update()
{
	// Reset temporary states
	for (S_Entity* entity : m_entitiesList)
		entity->getMovement().endUpdate();
	for (S_Entity* entity : m_entitiesList)
		entity->getCombat().endUpdate();
	
	// Generic tick
	for (S_Entity* entity : m_entitiesList)
		entity->update();

	// Simulate movement
	for (S_Entity* entity : m_entitiesList)
		entity->getMovement().update();

	// Simulate combat
	for (S_Entity* entity : m_entitiesList)
		entity->getCombat().update();
}

void S_WorldManager::registerNPC(const u16 npcid, const EntityType entityType)
{
	S_Entity* entity = new S_Entity_NPC((u16)(MAX_PLAYERS + npcid), entityType, R_Overworld);
	m_entities[entity->getUID()] = entity;
	m_entitiesList.push_back(entity);
	m_regions[entity->getMovement().region]->addEntity(*entity);
}

void S_WorldManager::registerPlayer(S_Connection& connection)
{
	m_regions[connection.getEntity().getMovement().region]->addConnection(connection);
	m_entities[connection.getUID()] = &connection.getEntity();
	m_entitiesList.push_back(&connection.getEntity());
}

void S_WorldManager::deregisterPlayer(S_Connection& connection)
{
	m_regions[connection.getEntity().getMovement().region]->removeConnection(connection);
	m_entities[connection.getUID()] = nullptr;
	for (auto iter = m_entitiesList.begin(); iter != m_entitiesList.end(); iter++)
		if ((*iter)->getUID() == connection.getUID())
		{
			m_entitiesList.erase(iter);
			return;
		}
	throw "Entity manager tried to deregister a connection that does not exist!";
}
