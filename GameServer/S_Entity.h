#pragma once
#include "S_Entity.h"
#include <queue>
#include "Global.h"
#include "S_CombatComponent.h"
#include "S_MovementComponent.h"

class S_Connection;
class S_Region;

class S_Entity
{
	friend class S_LoginManager;
	friend class S_Server;
public:

private:
	u16 m_id;
	EntityType m_entityType;
	S_Connection* m_connection; // nullptr if this is not a player.

protected:
	// Components
	S_CombatComponent m_combat;
	S_MovementComponent m_movement;

public:
	S_Entity(const u16 uid, const EntityType entityType, const Region region);
	~S_Entity();

	/// Called each tick of the server
	virtual void tick();

	//---- Getters / Setters ----//

	/// Returns the unique id of this entity
	const u16& getUID() const;

	/// Returns this entity's type
	const EntityType& getEntityType() const;
	
	/// Returns the Connection attached to this entity, or nullptr if this is not a player
	const S_Connection& getConnection() const;
	
	//---- Components ----//
	S_CombatComponent& getCombat();
	S_MovementComponent& getMovement();

	//----- Events ----- //

	/// Called when this entity respawns
	virtual void onRespawn();

	/// Called upon this entity's death
	virtual void onDeath();
};

