#include "S_LoginManager.h"
#include <filesystem>
#include <fstream>
#include "SaveState.h"

namespace fs = std::experimental::filesystem;

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

				// Load the players save file
				std::string filename = "saves/save_" + std::string(conn->username) + ".sav";
				//fstream file;
				//file.open(filename, ios::in | ios::out | ios::binary);
				//file.write("send nudez please", sizeof("send nudez please"));
				//file.close();

				fstream file;
				SaveState state;
				if (false)// sf::exists(filename))
				{
					printf("Loading user:%s's save.\n", conn->username);
					file.open(filename, ios::in | ios::out | ios::app | ios::binary);
					file.read((char*)(&state), sizeof(SaveState));

					// Check to see if the given password is incorrect
					if (strncmp(state.password, conn->password, 12) != 0)
					{
						printf("User:%s provided an incorrect password!\n", conn->username);
						conn->forceDisconnect = true;
					}
				}
				else
				{
					printf("Creating a save file for user:%s\n", conn->username);
					file.open(filename, ios::in | ios::out | ios::app | ios::binary);
					state = SaveState::newSave(conn);
					state.position.x += rand() % 4;
					state.position.y += rand() % 4;
					file.write((const char*)(&state), sizeof(SaveState));
				}
				SaveState::loadPlayersState(state, conn);
				file.close();

				// Add to the list of loaded players
				m_readyPlayers.push(conn);
			}
		}
		m_mutex_readyPlayers.unlock();
		m_mutex_connectionsToLoad.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}
