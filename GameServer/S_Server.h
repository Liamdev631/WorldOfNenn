// Includes & Preprocessor
#pragma once
#include "Global.h"
#include <enetpp\server.h>
#include <string>
#include <vector>
#include "RPacket.h"
#include "S_LoginManager.h"
#include "S_WorldManager.h"
#include "S_Entity_Player.h"

// Forward declaration
struct server_client;

// Class MPServer
class S_Server
{
public:
	u32 timestamp;
	//static S_WorldManager* WorldManager;

private:
	std::unique_ptr<enetpp::server<S_Entity_Player>> m_server;
	S_LoginManager* m_loginManager;
	S_WorldManager* m_worldManager;

	S_Entity_Player* m_connections[MAX_PLAYERS];
	std::vector<S_Entity_Player*> m_loadedPlayers;
	std::vector<S_Entity_Player*> m_playersToRemove;

public:
	S_Server();
	~S_Server();

	void start();
	void update(); // 12 times/sec, fixed
	void stop();
	
	S_WorldManager& getWorldManager() const;
	const std::vector<S_Entity_Player*>& getPlayers() const;

private:
	void clientInitFunc(S_Entity_Player& client, const char* ip);
	void onClientConnected(S_Entity_Player& client);
	void onClientDisconnected(S_Entity_Player& client_uid);
	void onDataRecieved(S_Entity_Player& client, RPacket answer);
	void transmitNewStates();
	void calculateNewState();
	void addConnectedPlayers();
	void disconnectPlayer(S_Entity_Player& connection);
};