#pragma once
#include "S_Server.h"

extern S_Server* g_server;

inline u32 getGameTime()
{
	return g_server->timestamp;
}