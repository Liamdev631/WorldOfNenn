#pragma once
#include "Global.h"
#include <map>
#include "S_Entity_Player.h"
#include "Items.h"

class S_WorldManager;

class S_Region
{
private:
	S_WorldManager& m_worldManager;

	// Entities
	std::map<u16, S_Entity*> m_entitiesList;
	std::map<u16, S_Entity_Player*> m_regionConnections;

	// Items
	std::vector<DropableItem> m_itemsList;
public:
	S_Region(S_WorldManager& worldManager);
	~S_Region();

	void update();

	std::map<u16, S_Entity_Player*>& getPlayers();
	
	// Entities
	std::map<u16, S_Entity*>& getEntities();
	void addEntity(S_Entity& newEntity);
	void addConnection(S_Entity_Player* connection);
	void removeConnection(S_Entity_Player* connection);

	// Items
	// Returns a const reference to the list of items.
	const std::vector<DropableItem>& getItems();

	// Places an item on the ground with the given parameters and notifies all nearby players.
	void addGroundItem(const DropableItem& item);

	// Removes the given item from the ground and notifies all nearby players.
	void removeGroundItem(std::vector<DropableItem>::const_iterator& item);

private:
};

