#pragma once
#include "Global.h"
#include "WPacket.h"

enum ObjectType : u16
{
	OT_FIRE_PIT = 0,
	OT_STOVE = 1,
	OT_TREE = 2,
};

union ObjectState
{
	struct
	{
		bool cut : 1;
	} tree;
};

typedef u16 ObjectUID;
struct Object
{
	ObjectUID uid;
	ObjectType type;
	ObjectState state;
	vec2s position;
	Region region;

	bool operator ==(const Object& rhs)
	{
		return uid == rhs.uid;
	}
};
