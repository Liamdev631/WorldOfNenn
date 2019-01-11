#include "S_Server.h"
#include <queue>
#include "WPacket.h"
#include "Packets.h"
#include "UseItem.h"

S_Server::S_Server()
{
	m_server = make_unique<enetpp::server<S_Entity_Player>>();
	for (int i = 0; i < MAX_PLAYERS; i++)
		m_connections[i] = nullptr;
	m_loginManager = new S_LoginManager();
	m_worldManager = new S_WorldManager();
	m_loadedPlayers.reserve(MAX_PLAYERS);
}

S_Server::~S_Server()
{
	m_server->stop_listening();
}

void S_Server::start()
{
	timestamp = 0;

	printf("Listening on port 801.\n");
	auto init_client_func = [&](S_Entity_Player& client, const char* ip) {
		clientInitFunc(client, ip);
	};

	m_server->start_listening(enetpp::server_listen_params<S_Entity_Player>()
		.set_max_client_count(256)
		.set_channel_count(1)
		.set_listen_port(801)
		.set_initialize_client_function(init_client_func));

	printf("Data tables loaded!\n");

	printf("Loading NPCs.\n");
	u16 npcsToAdd = 6;
	for (u16 i = 0; i < npcsToAdd; i++)
		m_worldManager->registerNPC(i, ET_RAT);
	printf("Loaded %u NPCs.\n", npcsToAdd);
}

void S_Server::addConnectedPlayers()
{
	auto newConnection = m_loginManager->popPlayer();
	while (newConnection)
	{
		m_worldManager->registerPlayer(newConnection);
		m_loadedPlayers.push_back(newConnection);
		auto& packet = newConnection->getBuffer();

		SP_Hello p = SP_Hello();
		p.timestamp = timestamp;
		p.uid = newConnection->uid;
		packet.write(p);
		newConnection->inventory.serialize(packet);

		newConnection->getMovement().blinkTo(newConnection->getMovement().getPos()); // This sends updates to nearby players

		printf("Client (%d) added as a player.\n", newConnection->uid);
		newConnection = m_loginManager->popPlayer();
	}
}

void S_Server::disconnectPlayer(S_Entity_Player& connection)
{
	auto iter = std::find(m_loadedPlayers.begin(), m_loadedPlayers.end(), &connection);
	assert(iter != m_loadedPlayers.end());
	m_loadedPlayers.erase(iter);
	m_worldManager->deregisterPlayer(&connection);

	// Send a message to the nearby players
	for (iter = m_loadedPlayers.begin(); iter != m_loadedPlayers.end(); iter++)
	{
		auto& buff = (*iter)->getBuffer();
		SP_RemoveEntity p = SP_RemoveEntity();
		p.uid = (*iter)->uid;
		buff.write(p);
	}

	printf("Client (%d) disconnected.\n", connection.uid);
	m_connections[connection.uid] = nullptr;
}

void S_Server::update()
{
	timestamp++;

	calculateNewState();

	transmitNewStates();

	// Send the packets
	for (auto c : m_server->get_connected_clients())
		if (c->getBuffer().getBytesWritten() > sizeof(u32))
		{
			c->getBuffer().getTimestamp() = timestamp; // Set the ts at the beginning of the packet
			m_server->send_packet_to(c->uid, 0, c->getBuffer().getData(), c->getBuffer().getBytesWritten(), ENET_PACKET_FLAG_RELIABLE);
			c->getBuffer().reset();
		}

}

void S_Server::calculateNewState()
{
	// Pull new players from the LoginManager
	addConnectedPlayers();

	// Consume events raised by the network worker thread
	auto on_client_connected = [&](S_Entity_Player& client) { onClientConnected(client); };
	auto on_client_disconnected = [&](S_Entity_Player& client) { onClientDisconnected(client); };
	auto on_client_data_received = [&](S_Entity_Player& client, const u8* data, size_t data_size) {
		onDataRecieved(client, RPacket(data, data_size));
	};
	m_server->consume_events(on_client_connected, on_client_disconnected, on_client_data_received);

	// Calculate new entity positions
	m_worldManager->update();
}

void S_Server::transmitNewStates()
{
	for (auto conn = m_loadedPlayers.begin(); conn != m_loadedPlayers.end(); conn++)
	{
		auto& player = *(*conn);
		WPacket& packet = player.getBuffer();
		S_Region& region = player.getMovement().getWorldRegion();

		// Send updated inventories
		if (player.inventory.dirty)
		{
			player.inventory.dirty = false;
			packet.write<u8>(0x03);
			packet.write<u16>(player.uid);
			player.inventory.serialize(packet);
		}
	}
}

void S_Server::stop()
{
	m_server->stop_listening();
}

S_WorldManager& S_Server::getWorldManager() const
{
	return *m_worldManager;
}

const std::vector<S_Entity_Player*>& S_Server::getPlayers() const
{
	return m_loadedPlayers;
}

void S_Server::clientInitFunc(S_Entity_Player& client, const char* ip)
{
	// Choose a uid for the new client
	u32 index = 0;
	while (m_connections[index]) // TODO: This should be limited by MAX_PLAYERS
		index++;
	m_connections[index] = &client;
	client.uid = index;
}

/// Called when a client connects to the server
void S_Server::onClientConnected(S_Entity_Player& client)
{
	printf("Client (%u) connected.\n", client.uid);
	m_loginManager->registerNewConnection(&client);
}

/// Called when a client disconnects from the server
void S_Server::onClientDisconnected(S_Entity_Player& client)
{
	disconnectPlayer(client);
}

/// Called when the server recieves data from a connected client
void S_Server::onDataRecieved(S_Entity_Player& player, RPacket packet)
{
	//printf("Data recieved from client with uid %u, length &u %u\n", client.uid, data_size);
	while (packet.getBytesRead() < packet.getSize())
	{
		const u8 header = packet.peek<u8>();
		switch (header)
		{
			case CP_AttackEntity_header:
			{
				// Request to attack
				const CP_AttackEntity& p = *packet.read<CP_AttackEntity>();
				player.getCombat().initiateCombat(*m_worldManager->getEntity(p.uid));
				continue;
			}

			case CP_MoveToDest_header:
			{
				// Player wants to move to a position
				const CP_MoveToDest& p = *packet.read<CP_MoveToDest>();
				player.getMovement().moveToPosition(p.pos);
				continue;
			}

			case CP_ItemPicked_header:
			{
				// Player wants to pick up an item
				const CP_ItemPicked& p = *packet.read<CP_ItemPicked>();
				player.pickUpItem(p.item);
				continue;
			}

			case CP_ItemDropped_header:
			{
				// Player wants to drop an item
				const CP_ItemDropped& p = *packet.read<CP_ItemDropped>();
				player.dropItemFromInventory(p.item, p.slot);
				continue;
			}

			case CP_SetRun_header:
			{
				const CP_SetRun& p = *packet.read<CP_SetRun>();
				player.getMovement().setRun(p.run);
				continue;
			}

			case CP_ChatText_header:
			{
				const CP_ChatText& p = *packet.read<CP_ChatText>();
				
				// Apply a chat filter
				std::wstring str = std::wstring(p.message);


				// Relay the message to nearby players
				for (auto& p : m_loadedPlayers)
					p->getBuffer().write(SP_ChatText(str, player.uid));
				continue;
			}

			case CP_UseItem_header:
			{
				const CP_UseItem p = *packet.read<CP_UseItem>();
				UseItem::use(player, p.itemType, p.slot);
				continue;
			}

			default:
			{
#ifdef _DEBUG
				throw exception("Unknown packet type reciever from client!");
				break;
#else
				printf("Unknown packet recieved!!!!\n");
				return;
#endif // _DEBUG
			}
		}
	}
}
