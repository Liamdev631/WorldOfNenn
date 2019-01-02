#pragma once
#include "Global.h"

enum MoveState : u8
{
	Idle = 0, // Make no actions
	Waypoint = 1, // Follow a series of waypoints
	Follow = 2, // Follow a entity until told otherwise
	Combat = 3, // Compelx movement associated with combat
	Guided = 4, // Follow a compelx series of waypoints, possibly with dialogue
};

struct MoveKey
{
	u32 timestamp;
	vec2s pos;
	u8 rot;
	u8 speed;
	MoveState state;

	MoveKey() = default;
	MoveKey(const u32& timestamp, const vec2s& position, const u8& rot, const MoveState& movestate, const u8& speed)
		: timestamp(timestamp), pos(pos), rot(rot), state(state), speed(speed)
	{

	}
};