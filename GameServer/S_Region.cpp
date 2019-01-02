#include "S_Region.h"
#include "S_WorldManager.h"
#include "S_Connection.h"
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
	m_entitiesList[entity.getUID()] = &entity;
}

void S_Region::addConnection(S_Connection& connection)
{
	m_regionConnections[connection.getUID()] = &connection;
	m_entitiesList[connection.getUID()] = &connection.getEntity();
}

void S_Region::removeConnection(S_Connection& connection)
{
	m_regionConnections.erase(connection.getUID());
	m_entitiesList.erase(connection.getUID());

}

std::map<u16, S_Entity*>& S_Region::getEntities()
{
	return m_entitiesList;
}

std::map<u16, S_Connection*>& S_Region::getConnections()
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
