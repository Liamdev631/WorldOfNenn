#pragma once
#include "C_Entity.h"
#include "Items.h"

enum TargetType : uint8_t
{
	TT_NONE,
	TT_ENTITY,
	TT_ITEM,
	TT_INV_ITEM,
};

struct Target
{
public:
	TargetType type;
	u32 uid;

	union
	{
		C_Entity* _entity;
		DropableItem _item;
	};

	Target()
		: type(TargetType::TT_NONE), uid(0)
	{ }
};
