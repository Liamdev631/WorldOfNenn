#pragma once
#include "S_Entity_NPC.h"
#include "S_Entity_Player.h"
#include "S_Region.h"
#include <vector>

class S_WorldManager
{
private:
	S_Entity** m_entities;
	std::vector<S_Entity*> m_entitiesList;
	S_Region** m_regions;

public:
	S_WorldManager();
	~S_WorldManager();

	S_Entity* getEntity(u16 index) const;
	std::vector<S_Entity*>& getEntities();
	S_Region& getRegion(const Region region) const;

	void update();
	S_Entity_NPC* registerNPC(const u16 npcid, const EntityType entityType);
	void registerPlayer(S_Entity_Player* connection);
	void deregisterPlayer(S_Entity_Player* connection);

private:
};

