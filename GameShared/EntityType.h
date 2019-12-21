#pragma once
#include "Global.h"

enum class EntityType : u16
{
	Admin = 0,
	Player = 1,
	Rat = 2,
	Pig = 3,
};

constexpr size_t NumEntityTypes = 4;
