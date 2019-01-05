#pragma once
#include "Global.h"
#include <queue>
#include "MoveKey.h"

#define WALK_SPEED 5
#define RUN_SPEED 3

class S_Region;
class S_Entity;

class S_MovementComponent
{
public:
	Region region;
	MoveKey moveKey;

private:
	S_Entity& owner;
	S_Entity* m_followEntity;
	std::queue<vec2s> m_waypoints;
	u8 m_moveTimer;
	bool m_running;

	bool m_hasMoved; // Desribes whether or not the player has changed position this tick
	bool m_forcePositionUpdate; // If this is set to true, all nearby players will recieve a position update for this entity this tick

public:
	S_MovementComponent(S_Entity& owner);
	~S_MovementComponent();

	void update();

	void stepInRandomDirection();

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

	// Attemps to change the running state of the player, notifying clients if neccesary
	void setRun(bool run);

	// Returns whether or not the player is running
	bool isRunning() const;
};

