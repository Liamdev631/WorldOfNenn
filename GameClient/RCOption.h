#pragma once
#include "Target.h"
#include "RightClickOption.h"

struct RCOption
{
	Target target;
	RightClickOption option;

	RCOption() : option(RCO_NONE) { }
	RCOption(const Target& target, const RightClickOption& option)
		: target(target), option(option)
	{

	}
};
