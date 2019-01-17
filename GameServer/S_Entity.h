#pragma once
#include "S_Entity.h"
#include <queue>
#include "Global.h"
#include "S_CombatComponent.h"
#include "S_MovementComponent.h"
#include "EntityType.h"

class S_Region;
class S_Entity_Player;

struct EntitySettings
{
	bool autoRetaliate;
};

class S_Entity
{
	friend class S_LoginManager;
	friend class S_Server;
public:
	u16 uid;

protected:
	// Components
	S_CombatComponent m_combat;
	S_MovementComponent m_movement;
	
	EntityType entityType;

public:
	S_Entity(const u16 uid, const EntityType entityType, const Region region);
	~S_Entity();

	/// Called each tick of the server
	virtual void update();

	//---- Getters / Setters ----//

	/// Returns this entity's type
	const EntityType& getEntityType() const;

	/// Returns this object cast as a player
	S_Entity_Player* asPlayer();
	
	//---- Components ----//
	S_CombatComponent& getCombat();
	S_MovementComponent& getMovement();

	//----- Events ----- //

	/// Called when this entity respawns
	virtual void onRespawn();

	/// Called upon this entity's death
	virtual void onDeath();
};

