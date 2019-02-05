#pragma once
#include "Global.h"
#include "WPacket.h"

enum ObjectType : u16
{
	OT_FIRE_PIT = 0,
	OT_STOVE = 1,
	OT_TREE = 2,
};

struct Object
{
	ObjectType type;
	vec2s position;
	union
	{
		struct tree
		{
			bool cut : 1;
		};
	};

	bool operator ==(const Object& rhs)
	{
		return type == rhs.type && position == rhs.position;
	}
};
