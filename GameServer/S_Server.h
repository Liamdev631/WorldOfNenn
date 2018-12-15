// Includes & Preprocessor
#pragma once
#include "Global.h"
#include <enetpp\server.h>
#include <string>
#include <vector>
#include "RPacket.h"
#include "S_Connection.h"
#include "S_LoginManager.h"
#include "S_WorldManager.h"

// Forward declaration
struct server_client;

// Class MPServer
class S_Server
{
public:
	u32 timestamp;
	//static S_WorldManager* WorldManager;

private:
	std::unique_ptr<enetpp::server<S_Connection>> m_server;
	S_LoginManager* m_loginManager;
	S_WorldManager* m_worldManager;

	S_Connection* m_connections[MAX_PLAYERS];
	std::vector<S_Connection*> m_loadedPlayers;
	std::vector<S_Connection*> m_playersToRemove;

public:
	S_Server();
	~S_Server();

	void start();
	void tick(); // 12 times/sec, fixed
	void stop();
	
	S_WorldManager& getWorldManager() const;
	const std::vector<S_Connection*>& getPlayers() const;

private:
	void clientInitFunc(S_Connection& client, const char* ip);
	void onClientConnected(S_Connection& client);
	void onClientDisconnected(S_Connection& client_uid);
	void onDataRecieved(S_Connection& client, RPacket answer);
	void transmitNewStates();
	void calculateNewState();
	void addConnectedPlayers();
	void disconnectPlayer(S_Connection& connection);
};