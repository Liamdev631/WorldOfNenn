#pragma once
#include "Global.h"
#include <queue>

class S_Region;
class S_Entity;

enum MoveState : u8
{
	Idle = 0, // Make no actions
	Waypoint = 1, // Follow a series of waypoints
	Follow = 2, // Follow a entity until told otherwise
	Combat = 3, // Compelx movement associated with combat
	Guided = 4, // Follow a compelx series of waypoints, possibly with dialogue
};

class S_MovementComponent
{
public:
	Region region;
	u8 rotation;
	vec2s position;

private:
	S_Entity& m_owner;
	MoveState m_moveState;
	S_Entity* m_followEntity;
	std::queue<vec2<u16>> m_waypoints;
	u8 m_moveTimer;
	u8 m_speed; // # of ticks needed to complete movement

	bool m_hasMoved; // Desribes whether or not the player has changed position this tick
	bool m_forcePositionUpdate; // If this is set to true, all nearby players will recieve a position update for this entity this tick

public:
	S_MovementComponent(S_Entity& owner);
	~S_MovementComponent();

	void tick();

	void blinkTo(const vec2s& position);

	/// Instructs the entity to move to a point if possible
	void moveToPosition(const vec2<u16>& position);

	/// Returns the movement type of this entity
	const MoveState& getMoveState() const;

	/// Returns the position of the entity this tick
	const vec2s& getPos() const;

	/// Returns the region where this entity is currently located
	S_Region& getWorldRegion() const;

	/// Resets the movement state of the entity. Sets to IDLE
	void resetMovement();

	/// Called at the beginning of every tick
	void reset();

	/// Returns true if the given entity is within 16 units of this entity
	bool isNear(const u16 entity) const;

	/// Returns true if the given entity is within 16 units of this entity
	bool isNear(S_Entity& entity) const;

	/// Returns true if the given entity is adjacent to this entity
	bool isNextTo(const u16 entity) const;

	/// Returns true if the given entity is adjacent to this entity
	bool isNextTo(S_Entity& entity) const;

	/// Returns true if the given position is within a certain distance
	bool isWithinDistance(const vec2s& position, int distance) const;

	/// Moves the entity one tile closer to the given position
	void stepTowards(const vec2s& target);

	/// Moves the entity one tile closer to the given entity
	void stepTowards(S_Entity& target);

	/// Instructs the entity to follow the given entity
	void startFollow(const u16 entity);

	/// Instructs the entity to follow the given entity
	void startFollow(S_Entity& entity);

	bool hasUpdate() const;

	void forcePositionUpdate();
};

