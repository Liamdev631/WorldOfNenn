#include "S_Region.h"
#include "S_WorldManager.h"
#include "Items.h"
#include "ServerPackets.h"

S_Region::S_Region(S_WorldManager& worldManager)
	: m_worldManager(worldManager)
{

}

S_Region::~S_Region()
{

}

void S_Region::update()
{

}

#pragma region Entities

void S_Region::addEntity(S_Entity& entity)
{
	m_entitiesList[entity.uid] = &entity;
}

void S_Region::addConnection(S_Entity_Player* connection)
{
	m_regionConnections[connection->uid] = connection;
	m_entitiesList[connection->uid] = connection;
}

void S_Region::removeConnection(S_Entity_Player* connection)
{
	m_regionConnections.erase(connection->uid);
	m_entitiesList.erase(connection->uid);
}

std::map<u16, S_Entity*>& S_Region::getEntities()
{
	return m_entitiesList;
}

std::map<u16, S_Entity_Player*>& S_Region::getPlayers()
{
	return m_regionConnections;
}

#pragma endregion

#pragma region Items

const std::vector<DropableItem>& S_Region::getItems()
{
	return m_itemsList;
}

void S_Region::addGroundItem(const DropableItem& item)
{
	m_itemsList.push_back(item);
	for (auto& connPair : m_regionConnections)
	{
		auto& buffer = connPair.second->getBuffer();
		buffer.write(SP_ItemDropped(item));
	}
}

void S_Region::removeGroundItem(std::vector<DropableItem>::const_iterator& item)
{
	// Tell the players
	for (auto& connPair : m_regionConnections)
	{
		auto& buffer = connPair.second->getBuffer();
		buffer.write(SP_ItemPicked(*item));
	}

	m_itemsList.erase(item);
}

#pragma endregion
