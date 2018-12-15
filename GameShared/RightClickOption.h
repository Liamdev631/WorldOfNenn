#pragma once
#include <cstdint>

enum RightClickOption : uint8_t
{
	RCO_NONE = 0,
	RCO_INSPECT = 1,
	RCO_ATTACK = 2,
	RCO_PICKUP = 3,
	RCO_USE = 4,
	RCO_DROP = 5,
	RCO_COUNT
};

static const std::wstring RightClickOptionNames[] =
{
	L"<none>",
	L"Inspect",
	L"Attack",
	L"Pick up",
	L"Use",
	L"Drop",
};
