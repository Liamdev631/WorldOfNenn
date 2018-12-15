#include "S_MovementComponent.h"
#include "S_GlobalServer.h"
#include "S_Region.h"
#include "S_Entity.h"
#include "Packets.h"

S_MovementComponent::S_MovementComponent(S_Entity& owner)
	: m_owner(owner), m_moveState(MoveState::Idle), m_hasMoved(false), m_moveTimer(0), region(R_Overworld), rotation(0)
{
	m_speed = 2;
}

S_MovementComponent::~S_MovementComponent()
{

}

void S_MovementComponent::tick()
{
	// Update position
	//printf("%d", m_moveState);
	switch (m_moveState)
	{
		case MoveState::Idle:
		{
			// Do nothing
			break;
		}

		case MoveState::Waypoint:
		{
			if (m_moveTimer++ < m_speed)
				break;
			m_moveTimer = 0;

			// Advance towards the next waypoint
			stepTowards(m_waypoints.front());
			if (m_waypoints.front() == position)
				m_waypoints.pop();

			// Set state to IDLE if we have reached the last waypoint
			if (m_waypoints.size() == 0)
				resetMovement();
			break;
		}

		case MoveState::Follow:
		{
			if (m_followEntity == nullptr) {
				resetMovement();
				break;
			}

			if (m_moveTimer++ != m_speed)
				break;
			m_moveTimer = 0;

			if (m_followEntity->getMovement().getPos() == position)
			{
				// Step off the target to a random direction
				switch (rand() % 4) {
					case 0: position.x += 1; break;
					case 1: position.x -= 1; break;
					case 2: position.y += 1; break;
					case 3: position.y -= 1; break;
				}
				m_hasMoved = true;
				break;
			}
			else
			{
				// Step towards the target
				int followDistance = 1;
				if (m_owner.getCombat().isInCombat)
					followDistance = m_owner.getCombat().getRange();
				if (!isWithinDistance(m_followEntity->getMovement().getPos(), followDistance))
					stepTowards(*m_followEntity);
			}
		}
	}
}

void S_MovementComponent::moveToPosition(const vec2<u16>& position)
{
	// Fresh movement state
	resetMovement();

	m_moveState = MoveState::Waypoint;
	m_waypoints.push(position);
}

void S_MovementComponent::blinkTo(const vec2s& position)
{
	this->position = position;
	auto& thisRegion = getWorldRegion();

	// If this entity is a player, we will need to tell it everything about the local game state
	if (m_owner.getEntityType() == ET_PLAYER || m_owner.getEntityType() == ET_ADMIN)
	{
		auto& packetBuffer = m_owner.getConnection().getBuffer();
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
			addEntitiesPacketElem.move.pos = otherEntity->getMovement().position;
			addEntitiesPacketElem.move.rot = otherEntity->getMovement().rotation;
		}
	}

	// Tell nearby players that the entity has appeared
	for (auto iter = thisRegion.getConnections().begin(); iter != thisRegion.getConnections().end(); iter++)
	{
		if (iter->second == &m_owner.getConnection())
			continue;
		auto& buffer = iter->second->getBuffer();

		SP_EntityStatus& addEntitiesPacket = *buffer.writePacket<SP_EntityStatus>();
		addEntitiesPacket.numEntities = 1;

		SP_EntityStatus_Elem& addEntitiesPacketElem = *buffer.writePacket<SP_EntityStatus_Elem>();
		addEntitiesPacketElem.uid = m_owner.getUID();
		addEntitiesPacketElem.entityType = m_owner.getEntityType();
		addEntitiesPacketElem.move.pos = position;
		addEntitiesPacketElem.move.rot = rotation;
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
	int deltaX = entityPos.x - position.x, deltaY = entityPos.y - position.y;
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
	int deltaX = entityPos.x - position.x, deltaY = entityPos.y - position.y;
	return deltaX <= 1 && deltaX >= -1 && deltaY <= 1 && deltaY >= -1;
}

bool S_MovementComponent::isWithinDistance(const vec2s& p, int distance) const
{
	return ((position.x - p.x <= distance && position.x - p.x >= -distance) &
		(position.y - p.y <= distance && position.y - p.y >= -distance));
}

const MoveState& S_MovementComponent::getMoveState() const
{
	return m_moveState;
}

const vec2s& S_MovementComponent::getPos() const
{
	return position;
}

void S_MovementComponent::resetMovement()
{
	while (m_waypoints.size() > 0)
		m_waypoints.pop();
	m_moveState = MoveState::Idle;
	m_moveTimer = 0;
}

void S_MovementComponent::stepTowards(const vec2s& target)
{
	vec2s prevPosition = position;
	// Move 1 tile towards the target
	if (position.x > target.x)
		position.x--;
	else if (position.x < target.x)
		position.x++;

	if (position.y > target.y)
		position.y--;
	else if (position.y < target.y)
		position.y++;

	// Calculate the new rotation
	if (m_owner.getCombat().isInCombat)
	{
		// Face the combat target
		rotation = directionFromTo(position, m_owner.getCombat().target->getMovement().getPos());
	}
	else
	{
		// Same as direction of movement
		rotation = directionFromTo(prevPosition, position);
	}

	// Transmit the new position
	auto nearbyPlayers = getWorldRegion().getConnections();
	SP_EntityMoved p = SP_EntityMoved();
	p.uid = m_owner.getUID();
	p.move.pos = position;
	p.move.rot = rotation;
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
	m_moveState = MoveState::Follow;
	m_followEntity = g_server->getWorldManager().getEntity(entity);
}

void S_MovementComponent::startFollow(S_Entity& entity)
{
	m_moveState = MoveState::Follow;
	m_followEntity = &entity;
}

S_Region& S_MovementComponent::getWorldRegion() const
{
	return g_server->getWorldManager().getRegion(region);
}

void S_MovementComponent::reset()
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
