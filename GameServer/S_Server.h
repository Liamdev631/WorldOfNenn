// Includes & Preprocessor
#pragma once
#include "Global.h"
#include <enetpp/server.h>
#include <string>
#include <vector>
#include "RPacket.h"
#include "S_LoginManager.h"
#include "S_WorldManager.h"
#include "S_Entity_Player.h"

using namespace std;

// Forward declaration
struct server_client;

// Class MPServer
class S_Server
{
public:
	u32 timestamp;
	//static S_WorldManager* WorldManager;

private:
	// The ENET server. Handles the tcp/udp connections with the clients.
	enetpp::server<S_Entity_Player> m_server;
	
	// Manages the login process.
	S_LoginManager::Ptr m_loginManager;
	
	// The world that the server is simulating
	S_World::Ptr m_worldManager;

	S_Entity_Player* m_connections[MAX_PLAYERS];
	std::vector<S_Entity_Player*> m_loadedPlayers;
	std::vector<S_Entity_Player*> m_playersToRemove;

public:
	S_Server();
	~S_Server();

	void start();
	void update(); // 12 times/sec, fixed
	void stop();
	
	S_World& getWorldManager() const;
	const std::vector<S_Entity_Player*>& getPlayers() const;

private:
	void clientInitFunc(S_Entity_Player& client, const char* ip);
	void onClientConnected(S_Entity_Player& client);
	void onClientDisconnected(S_Entity_Player& client_uid);
	void onDataRecieved(S_Entity_Player& client, RPacket answer);
	void transmitNewStates();
	void calculateNewState();
	void addConnectedPlayers();
	void disconnectPlayer(u16 uid);
};