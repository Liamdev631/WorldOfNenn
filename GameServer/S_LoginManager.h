/// LoginManager.h
#pragma once
#include <queue>
#include <mutex>
#include <thread>
#include "S_Entity_Player.h"

class S_LoginManager
{
public:
	S_LoginManager();
	~S_LoginManager();

	void registerNewConnection(S_Entity_Player* newPeer);
	S_Entity_Player* popPlayer();

private:
	// New connections that must have their player info loaded
	std::queue<S_Entity_Player*> m_connectionsToLoad;
	std::mutex m_mutex_connectionsToLoad;

	// Loaded player objects that are ready to be injected into the world
	std::queue<S_Entity_Player*> m_readyPlayers;
	std::mutex m_mutex_readyPlayers;

	std::thread m_thread_loadPlayers;
	void loadPlayers();
	bool m_threadRunning; // set this so true and jon the thread to exit it safely
};
