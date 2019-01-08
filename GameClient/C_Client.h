#pragma once
#include "Global.h"
#include <enetpp\client.h>
#include "WPacket.h"
#include "C_WorldManager.h"
#include "C_ItemManager.h"
#include "Loader.h"
#include "Items.h"
#include <string>
#include "GameTime.h"
#include "ItemContainer.h"

constexpr auto SERVER_IP = "localhost";// "localhost";
constexpr bool LOG_PACKET_HEADERS = false;

enum ConnectionState
{
	Disconnected,
	SayingHello,
	Connected
};

class C_Client
{
private:
	ConnectionState m_connectionState;
	std::unique_ptr<enetpp::client> m_client;
	std::mutex m_entityMutex;
	WPacket* m_packetBuffer;
	GameTime m_timer; // For local timing
	sf::Clock clock;

	Inventory m_playerInventory;
	ExperienceTable m_playerExperienceTable;

public:
	C_Client();
	~C_Client();

	WPacket& getPacket() const;
	const Inventory& getPlayerInventory() const;
	const ExperienceTable& getPlayerExperienceTable() const;

	static C_Client& get()
	{
		static C_Client c;
		return c;
	}

	const GameTime& getGameTime() const;

	// Initializes recources required for the game, including networking.
	void start();
	
	// Runs a standard game update.
	// @return: true if the game should continue running. False otherwise.
	bool update();	// 60 times/sec, variable

	// Abruptly stops networking and deletes managed recources
	void stop();

private:
	void onConnect();
	void onDisconnect();
	void onDataRecieved(const u8* data, size_t size);
	void processPacket(const u8 header, RPacket &packet);
};
