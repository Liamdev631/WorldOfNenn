#include "C_Client.h"
#include "Packets.h"
#include <chrono>
#include <thread>
#include "C_WorldScene.h"
#include "RunButton.h"

C_Client::C_Client()
{
	m_connectionState = ConnectionState::Disconnected;
	m_client = make_unique<enetpp::client>();
	//m_gui = new C_WindowComponent();
	m_packetBuffer = new WPacket(512);
	m_timer.totalTime = 0;
	m_timer.deltaTime = 0;

	// Load all the game info preemptively
	Loader::get();
}

C_Client::~C_Client()
{

}

void C_Client::start()
{
	printf("Client is looking for server!\n");
	m_client->connect(enetpp::client_connect_params()
		.set_channel_count(1)
		.set_server_host_name_and_port(SERVER_IP, 801));
	m_connectionState = ConnectionState::SayingHello;
	//m_gui->loadResources();
}

bool C_Client::update()
{
	if (!m_client.get()->is_connecting_or_connected())
		return true;

	auto start = std::chrono::high_resolution_clock::now();

	// Consume events raised by worker thread
	auto on_connected = [&]() { onConnect(); };
	auto on_disconnected = [&]() { onDisconnect(); };
	auto on_data_received = [&](const u8* data, size_t size) {
		onDataRecieved(data, size);
	};
	m_client->consume_events(on_connected, on_disconnected, on_data_received);

	//if (!m_gui->update(m_timer))
	//	return false;
	C_WorldManager::get().update(m_timer);
	SceneManager::get().update(m_timer);

	// END GAME LOGIC

	// Send the packet buffer
	if (m_packetBuffer->getBytesWritten() > 0)
	{
		m_client->send_packet(0, m_packetBuffer->getData(), m_packetBuffer->getBytesWritten(), ENET_PACKET_FLAG_RELIABLE);
		m_packetBuffer->reset();
	}

	// Render
	SceneManager::get().draw();

	auto end = std::chrono::high_resolution_clock::now();
	auto dt = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	dt /= 1000000000.f; // conversion to seconds
	m_timer.addDeltaTime(dt);
	return true;
} 

void C_Client::stop()
{
	m_client->disconnect();
}

WPacket& C_Client::getPacket() const
{
	return *m_packetBuffer;
}

const Inventory& C_Client::getPlayerInventory() const
{
	return m_inventory;
}

void C_Client::onConnect()
{
	printf("Client connected to server!\n");
	m_connectionState = ConnectionState::Connected;
}

void C_Client::onDisconnect()
{
	printf("Client disconnected from server!\n");
}

void C_Client::onDataRecieved(const u8* data, size_t size)
{
	m_entityMutex.lock();
	RPacket packet = RPacket(data, size);
	//printf("timestamp: %u\n", packet.getTimestamp());

	while (packet.getBytesRead() < packet.getSize())
	{
		const u8 header = packet.peek<u8>();
		processPacket(header, packet);
	}

	m_entityMutex.unlock();
}

const GameTime& C_Client::getGameTime() const
{
	return m_timer;
}

void C_Client::processPacket(const u8 header, RPacket &packet)
{
	if (LOG_PACKET_HEADERS) {
		printf("\n");
	}

	switch (header)
	{
	case SP_Hello_header:
	{
		// 'Hello' Packet
		const SP_Hello& p = *packet.read<SP_Hello>();
		if (LOG_PACKET_HEADERS) {
			printf("Processing P_Hello. uid:%u time:%u\n", p.uid, p.timestamp);
			printBytes(p);
		}
		m_connectionState = ConnectionState::Connected;
		C_WorldManager::get().setThisUID(p.uid);
		m_timer.totalTime = p.timestamp;
		m_inventory.deserialize(packet);
		break;
	}

	case SP_EntityStatus_header:
	{
		// Entity Status
		const SP_EntityStatus& p = *packet.read<SP_EntityStatus>();
		if (LOG_PACKET_HEADERS) {
			printf("Processing P_EntityStatus. entities:%u\n", p.numEntities);
			printBytes(p);
		}
		assert(p.numEntities >= 1);
		for (int i = 0; i < p.numEntities; i++)
		{
			const SP_EntityStatus_Elem& p2 = *packet.read<SP_EntityStatus_Elem>();
			C_Entity* entity = C_WorldManager::get().getEntity(p2.uid);
			entity->entityType = p2.entityType;
			entity->addMoveKey(p2.move);
			printf("speed:%u\n", p2.move.speed);
			if (LOG_PACKET_HEADERS) {
				printf("Processing P_EntityStatus_Elem elem:%u uid:%#06x type:%#04x pos:(%#06x,%#06x)\n", i, p2.uid, p2.entityType, p2.move.pos.x, p2.move.pos.y);
				printBytes(p);
			}
		}
		break;
	}

	case SP_RemoveEntity_header:
	{
		// Remove entity
		const SP_RemoveEntity& p = *packet.read<SP_RemoveEntity>();
		C_WorldManager::get().removeEntity(p.uid);
		if (LOG_PACKET_HEADERS) {
			printf("Processing P_RemoveEntity. uid:%u\n", p.uid);
			printBytes(p);
		}
		break;
	}

	case SP_Inventory_header:
	{
		// Inventory
		const SP_Inventory& p = *packet.read<SP_Inventory>();
		m_inventory.deserialize(packet);
		if (LOG_PACKET_HEADERS) {
			printf("Processing P_Inventory. inventory: \n");
			printBytes(p);
		}
		break;
	}

	case SP_EntityTookDamage_header:
	{
		// Entity took damage
		const SP_EntityTookDamage& p = *packet.read<SP_EntityTookDamage>();
		if (LOG_PACKET_HEADERS) {
			printf("Processing P_EntityTookDamage. defender:%u damage:%u health:%u/%u dead:%s\n", p.defender, p.damage, p.combatState.currentHealth, p.combatState.maxHealth, p.combatState.dead ? "yes" : "no");
			printBytes(p);
		}
		C_Entity* defenderEntity = C_WorldManager::get().getEntity(p.defender);
		if (defenderEntity == nullptr)
			break;
		defenderEntity->combatState = p.combatState;
		defenderEntity->hitMarkers.push_back(HitMarker(p.damage));
		break;
	}

	case SP_ItemPicked_header:
	{
		const SP_ItemPicked& p = *packet.read<SP_ItemPicked>();
		if (LOG_PACKET_HEADERS) {
			printf("Processing P_ItemPicked. item:%u count:%u\n", p.item.stack.type, p.item.stack.count);
			printBytes(p);
		}
		C_ItemManager::get().pickUpItem(p.item);
		break;
	}

	case SP_ItemDropped_header:
	{
		const SP_ItemDropped& p = *packet.read<SP_ItemDropped>();
		if (LOG_PACKET_HEADERS) {
			auto itemName = Loader::get().getItemName(p.item.stack.type).c_str();
			printf("Processing P_ItemDropped. item:%ls count:%u pos:(%u,%u)\n", itemName, p.item.stack.count, p.item.pos.x, p.item.pos.y);
			printBytes(p);
		}
		C_ItemManager::get().dropItemFromInventory(p.item);
		break;
	}

	case SP_EntityMoved_header:
	{
		const SP_EntityMoved& p = *packet.read<SP_EntityMoved>();
		auto entity = C_WorldManager::get().getEntity(p.uid);
		if (!entity)
			break;
		entity->addMoveKey(p.move);

		if (LOG_PACKET_HEADERS) {
			printf("Processing SP_EntityMoved. pos:(%u,%u) rot:%u\n", p.move.pos.x, p.move.pos.y, p.move.rot);
			printBytes(p);
		}

		break;
	}

	case SP_SetRun_header:
	{
		const auto& p = *packet.read<SP_SetRun>();
		RunButton::get().setActive(p.run);
		if (LOG_PACKET_HEADERS)
			printf("Processing SP_SetRun. (run:%u)\n", p.run);
		break;
	}

	case SP_ChatText_header:
	{
		const SP_ChatText& p = *packet.read<SP_ChatText>();
		std::wstring str = std::wstring(p.chatText);

		// Send the text to chat
		std::wstringstream textBuilder;
		textBuilder << L"Player " << p.speaker << L": " << str;
		Chatbox::get().addText(textBuilder.str());

		// Set the floating text above the player
		auto speaker = C_WorldManager::get().getEntity(p.speaker);
		//speaker->setFloatingText(str);

		break;
	}

	case SP_ExperienceTable_header:
	{
		const SP_ExperienceTable& p = *packet.read<SP_ExperienceTable>();
		printf("xp\n");
		break;
	}

	default:
	{
		printf("INVALID PACKET RECIEVED! header:%u\n First 12 bytes: ", header);
		printBytes(&packet.peek<u8>(), 12);
		break;
	}
	}
}
