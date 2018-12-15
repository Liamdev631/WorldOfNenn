#pragma once
#include <cstdint>

struct GameTime
{
	uint32_t totalTime; // ms
	float deltaTime; // seconds

	void addDeltaTime(const float& dt)
	{
		deltaTime = dt;
		totalTime += (uint32_t)(1000.f * dt);
	}
};