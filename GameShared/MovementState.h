#pragma once

#pragma pack(push, 1)
struct MovementState
{
	vec2s pos;
	u8 rot;

	MovementState()
		: pos(vec2s(0, 0)), rot(0)
	{

	}
};
#pragma pack(pop)