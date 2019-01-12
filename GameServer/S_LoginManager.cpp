#include "S_LoginManager.h"

S_LoginManager::S_LoginManager()
{
	m_thread_loadPlayers = std::thread(&S_LoginManager::loadPlayers, this);
	m_thread_loadPlayers.detach();
}

S_LoginManager::~S_LoginManager()
{

}

void S_LoginManager::registerNewConnection(S_Entity_Player* newPeer)
{
	m_mutex_connectionsToLoad.lock();
	{
		m_connectionsToLoad.push(newPeer);
	}
	m_mutex_connectionsToLoad.unlock();
}

S_Entity_Player* S_LoginManager::popPlayer()
{
	S_Entity_Player* ret = nullptr;
	m_mutex_readyPlayers.lock();
	{
		if (m_readyPlayers.size() > 0)
		{
			ret = m_readyPlayers.front();
			m_readyPlayers.pop();
		}
	}
	m_mutex_readyPlayers.unlock();
	return ret;
}

void S_LoginManager::loadPlayers()
{
	// Constantly work on loading players in the queue
	while (true)
	{
		m_mutex_connectionsToLoad.lock();
		m_mutex_readyPlayers.lock();
		{
			while (m_connectionsToLoad.size() > 0)
			{
				// Take a player off the stack
				auto conn = m_connectionsToLoad.front();
				m_connectionsToLoad.pop();

				// Load the players character
				conn->getMovement().region = R_Overworld;
				conn->getMovement().blinkTo(vec2s(115, 115));

				// Add to the list of loaded players
				m_readyPlayers.push(conn);
			}
		}
		m_mutex_readyPlayers.unlock();
		m_mutex_connectionsToLoad.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}
