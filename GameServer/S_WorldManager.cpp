#include "S_WorldManager.h"
#include <assert.h>
#include "CSVReader.h"

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

	// Load objects into regions
	CSVReader reader;
	reader.open("assets/data/ObjectInstanceData.csv");
	reader.readNextRow();
	int counter = 0;
	while (reader.size() > 1)
	{
		Object o;
		o.uid = counter++;
		o.type = (ObjectType)std::stoi(reader[0]);
		o.position.x = (u16)std::stoi(reader[1]);
		o.position.y = (u16)std::stoi(reader[2]);
		o.region = (Region)std::stoi(reader[3]);
		m_regions[o.region]->injectObject(o);
		reader.readNextRow();
	}
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

S_Entity_NPC* S_WorldManager::registerNPC(const u16 npcid, const EntityType entityType)
{
	S_Entity_NPC* entity = new S_Entity_NPC((u16)(MAX_PLAYERS + npcid), entityType, R_Overworld);
	m_entities[entity->uid] = entity;
	m_entitiesList.push_back(entity);
	m_regions[entity->getMovement().region]->addEntity(*entity);
	return entity;
}

void S_WorldManager::registerPlayer(S_Entity_Player* connection)
{
	m_regions[connection->getMovement().region]->addConnection(connection);
	m_entities[connection->uid] = connection;
	m_entitiesList.push_back(connection);
}

void S_WorldManager::deregisterPlayer(S_Entity_Player* connection)
{
	m_regions[connection->getMovement().region]->removeConnection(connection);
	m_entities[connection->uid] = nullptr;
	for (auto iter = m_entitiesList.begin(); iter != m_entitiesList.end(); iter++)
		if ((*iter)->uid == connection->uid)
		{
			m_entitiesList.erase(iter);
			return;
		}
	throw "Entity manager tried to deregister a connection that does not exist!";
}
