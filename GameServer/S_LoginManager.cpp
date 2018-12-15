#include "S_LoginManager.h"

S_LoginManager::S_LoginManager()
{
	m_thread_loadPlayers = std::thread(&S_LoginManager::loadPlayers, this);
	m_thread_loadPlayers.detach();
}

S_LoginManager::~S_LoginManager()
{

}

void S_LoginManager::registerNewConnection(S_Connection* newPeer)
{
	m_mutex_connectionsToLoad.lock();
	{
		m_connectionsToLoad.push(newPeer);
	}
	m_mutex_connectionsToLoad.unlock();
}

S_Connection* S_LoginManager::popPlayer()
{
	S_Connection* ret = nullptr;
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
				S_Connection* conn = m_connectionsToLoad.front();
				S_Entity* newPlayer = new S_Entity((u16)conn->getUID(), ET_PLAYER, R_Overworld);
				newPlayer->getMovement().position = vec2<u16>(40 + rand() % 10, 40 + rand() % 10);
				conn->setPlayer(*newPlayer);
				m_connectionsToLoad.pop();
				m_readyPlayers.push(conn);
			}
		}
		m_mutex_readyPlayers.unlock();
		m_mutex_connectionsToLoad.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}
