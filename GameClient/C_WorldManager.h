#pragma once
#include "Global.h"
#include <vector>
#include "C_Entity.h"
#include "RPacket.h"
#include "Packets.h"
#include "GameTime.h"

class C_WorldManager
{
private:
	std::vector<C_Entity*> m_activeEntities;
	u16 uid;

public:
	C_WorldManager();
	~C_WorldManager();

	// Singleton
	static C_WorldManager& get()
	{
		static C_WorldManager w;
		return w;
	}

	// Clears the entity list
	void clearEntities();

	// Removes and entity from the entity list
	void removeEntity(const u16 uid);

	// Returns a list of all the loaded entities
	std::vector<C_Entity*>& getActiveEntities();

	// Returns an entity with a given uid
	C_Entity* getEntity(const u16 uid);

	// Returns the Entity representing this player
	C_Entity* getThisEntity();

	// Sets the uid of this player, given by the server
	void setThisUID(const u16 uid);

	// Runs an update of the EntityManager
	void update(const GameTime& gameTime);

private:
};

