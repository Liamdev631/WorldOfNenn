#pragma once
#include "Global.h"
#include <map>
#include "S_Entity.h"
#include "Items.h"

class S_WorldManager;
class S_Entity;
class S_Connection;

class S_Region
{
private:
	S_WorldManager& m_worldManager;

	// Entities
	std::map<u16, S_Entity*> m_entitiesList;
	std::map<u16, S_Connection*> m_regionConnections;

	// Items
	std::vector<DropableItem> m_itemsList;
public:
	S_Region(S_WorldManager& worldManager);
	~S_Region();

	void tick();

	std::map<u16, S_Connection*>& getConnections();
	
	// Entities
	std::map<u16, S_Entity*>& getEntities();
	void addEntity(S_Entity& newEntity);
	void addConnection(S_Connection& newConnection);
	void removeConnection(S_Connection& entity);

	// Items
	// Returns a const reference to the list of items.
	const std::vector<DropableItem>& getItems();

	// Places an item on the ground with the given parameters and notifies all nearby players.
	void addGroundItem(const DropableItem& item);

	// Removes the given item from the ground and notifies all nearby players.
	void removeGroundItem(std::vector<DropableItem>::const_iterator& item);

private:
};

