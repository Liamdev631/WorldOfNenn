#include "S_MovementComponent.h"
#include "S_GlobalServer.h"
#include "S_Region.h"
#include "S_Entity.h"
#include "Packets.h"

S_MovementComponent::S_MovementComponent(S_Entity& owner)
	: m_owner(owner), m_hasMoved(false), m_moveTimer(0), region(R_Overworld),
	m_running(false)
{
	moveKey.speed = 2;
}

S_MovementComponent::~S_MovementComponent()
{

}

void S_MovementComponent::update()
{
	if (moveKey.state == MoveState::Idle)
		return;

	moveKey.speed = m_running ? 4 : 6;
	if (m_moveTimer++ < moveKey.speed)
		return;

	switch (moveKey.state)
	{
		case MoveState::Waypoint:
		{
			// Advance towards the next waypoint
			auto& nextWaypoint = m_waypoints.front();
			stepTowards(nextWaypoint);

			// Check if we have reached the next waypoint or not
			if (nextWaypoint == moveKey.pos)
				m_waypoints.pop();

			// Set state to IDLE if we have reached the last waypoint
			if (m_waypoints.size() == 0)
				resetMovement();
			break;
		}

		case MoveState::Follow:
		{
			// If the entity doesn't exist anymore, stop following it
			//if (m_followEntity == nullptr) {
			//	resetMovement();
			//	break;
			//}

			if (m_followEntity->getMovement().getPos() == moveKey.pos)
			{
				// Step off the target to a random direction
				stepInRandomDirection();
				break;
			}
			else
			{
				// Step towards the target until in follow distance
				int followDistance = 1;
				if (m_owner.getCombat().isInCombat)
					followDistance = m_owner.getCombat().getRange();
				if (!isWithinDistance(m_followEntity->getMovement().getPos(), followDistance))
					stepTowards(*m_followEntity);
			}
			break;
		}

		default:
			throw;
	}
}

void S_MovementComponent::stepInRandomDirection()
{
	switch (rand() % 4) {
		case 0: moveKey.pos.x += 1; break;
		case 1: moveKey.pos.x -= 1; break;
		case 2: moveKey.pos.y += 1; break;
		case 3: moveKey.pos.y -= 1; break;
	}
	m_hasMoved = true;
}

void S_MovementComponent::moveToPosition(const vec2<u16>& pos)
{
	// Fresh movement state
	resetMovement();
	moveKey.state = MoveState::Waypoint;
	m_waypoints.push(pos);
}

void S_MovementComponent::blinkTo(const vec2s& pos)
{
	this->moveKey.pos = pos;
	auto& thisRegion = getWorldRegion();

	// If this entity is a player, we will need to tell it everything about the local game state
	if (m_owner.getEntityType() == ET_PLAYER || m_owner.getEntityType() == ET_ADMIN)
	{
		auto& packetBuffer = m_owner.getConnection()->getBuffer();
		SP_EntityStatus& addEntitiesPacket = *packetBuffer.writePacket<SP_EntityStatus>();
		assert(thisRegion.getEntities().size() <= 0xFF); // More than 255 entities in the players surroundings. Rediculous!
		addEntitiesPacket.numEntities = (u8)thisRegion.getEntities().size();
		for (auto iter = thisRegion.getEntities().begin();
			iter != thisRegion.getEntities().end(); iter++)
		{
			auto otherEntity = iter->second;
			SP_EntityStatus_Elem& addEntitiesPacketElem = *packetBuffer.writePacket<SP_EntityStatus_Elem>();
			addEntitiesPacketElem.uid = otherEntity->getUID();
			addEntitiesPacketElem.entityType = otherEntity->getEntityType();
			addEntitiesPacketElem.move = otherEntity->getMovement().moveKey;
		}
	}

	// Tell nearby players that the entity has appeared
	for (auto iter = thisRegion.getConnections().begin(); iter != thisRegion.getConnections().end(); iter++)
	{
		if (iter->second == m_owner.getConnection())
			continue;
		auto& buffer = iter->second->getBuffer();

		SP_EntityStatus& addEntitiesPacket = *buffer.writePacket<SP_EntityStatus>();
		addEntitiesPacket.numEntities = 1;

		SP_EntityStatus_Elem& addEntitiesPacketElem = *buffer.writePacket<SP_EntityStatus_Elem>();
		addEntitiesPacketElem.uid = m_owner.getUID();
		addEntitiesPacketElem.entityType = m_owner.getEntityType();
		addEntitiesPacketElem.move = moveKey;
	}
}

bool S_MovementComponent::isNear(u16 entity) const
{
	auto otherEntity = g_server->getWorldManager().getEntity(entity);
	assert(&otherEntity != nullptr);
	return isNear(*otherEntity);
}

bool S_MovementComponent::isNear(S_Entity& entity) const
{
	const vec2s& entityPos = entity.getMovement().getPos();
	int deltaX = entityPos.x - moveKey.pos.x, deltaY = entityPos.y - moveKey.pos.y;
	return deltaX <= 15 && deltaX >= -16 && deltaY <= 15 && deltaY >= -16;
}

bool S_MovementComponent::isNextTo(u16 entity) const
{
	S_Entity* otherEntity = g_server->getWorldManager().getEntity(entity);
	assert(otherEntity != nullptr);
	return isNextTo(*otherEntity);
}

bool S_MovementComponent::isNextTo(S_Entity& entity) const
{
	const vec2s& entityPos = entity.getMovement().getPos();
	int deltaX = entityPos.x - moveKey.pos.x, deltaY = entityPos.y - moveKey.pos.y;
	return deltaX <= 1 && deltaX >= -1 && deltaY <= 1 && deltaY >= -1;
}

bool S_MovementComponent::isWithinDistance(const vec2s& p, int distance) const
{
	return ((moveKey.pos.x - p.x <= distance && moveKey.pos.x - p.x >= -distance) &
		(moveKey.pos.y - p.y <= distance && moveKey.pos.y - p.y >= -distance));
}

const MoveState& S_MovementComponent::getMoveState() const
{
	return moveKey.state;
}

const vec2s& S_MovementComponent::getPos() const
{
	return moveKey.pos;
}

void S_MovementComponent::resetMovement()
{
	while (m_waypoints.size() > 0)
		m_waypoints.pop();
	moveKey.state = MoveState::Idle;
	m_moveTimer = 99;
}

void S_MovementComponent::stepTowards(const vec2s& target)
{
	vec2s prevPos = moveKey.pos;
	// Move 1 tile towards the target
	if (moveKey.pos.x > target.x)
		moveKey.pos.x--;
	else if (moveKey.pos.x < target.x)
		moveKey.pos.x++;

	if (moveKey.pos.y > target.y)
		moveKey.pos.y--;
	else if (moveKey.pos.y < target.y)
		moveKey.pos.y++;

	// Calculate the new rotation
	if (m_owner.getCombat().isInCombat)
	{
		// Face the combat target
		moveKey.rot = directionFromTo(moveKey.pos, m_owner.getCombat().target->getMovement().getPos());
	}
	else
	{
		// Same as direction of movement
		moveKey.rot = directionFromTo(prevPos, moveKey.pos);
	}

	// Transmit the new m_moveKey.pos
	auto nearbyPlayers = getWorldRegion().getConnections();
	SP_EntityMoved p = SP_EntityMoved();
	p.uid = m_owner.getUID();
	p.move = moveKey;
	for (auto iter = nearbyPlayers.begin(); iter != nearbyPlayers.end(); iter++)
	{
		auto& packet = iter->second->getBuffer();
		packet.write(p);
	}

	m_hasMoved = true;
}

void S_MovementComponent::stepTowards(S_Entity& target)
{
	stepTowards(target.getMovement().getPos());
}

void S_MovementComponent::startFollow(u16 entity)
{
	resetMovement();
	moveKey.state = MoveState::Follow;
	m_followEntity = g_server->getWorldManager().getEntity(entity);
}

void S_MovementComponent::startFollow(S_Entity& entity)
{
	moveKey.state = MoveState::Follow;
	m_followEntity = &entity;
}

S_Region& S_MovementComponent::getWorldRegion() const
{
	return g_server->getWorldManager().getRegion(region);
}

void S_MovementComponent::endUpdate()
{
	m_hasMoved = false;
	m_forcePositionUpdate = false;
}

bool S_MovementComponent::hasUpdate() const
{
	return m_hasMoved || m_forcePositionUpdate;
}

void S_MovementComponent::forcePositionUpdate()
{
	m_forcePositionUpdate = true;
}

void S_MovementComponent::setRun(bool run)
{
	m_running = run;
	if (m_owner.getConnection())
		m_owner.getConnection()->getBuffer().write(SP_SetRun(m_running));
}

bool S_MovementComponent::isRunning() const
{
	return m_running;
}
