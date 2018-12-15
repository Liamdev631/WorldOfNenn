#pragma once
#include <string>
#include "Global.h"
#include "S_Entity.h"
#include "Items.h"

class S_Entity; // TODO: Figure out why this is needed

class S_Connection
{
public:
	Inventory inventory;

private:
	u32 m_uid;
	S_Entity* m_player;
	WPacket* m_buffer;

public:
	S_Connection();
	~S_Connection();

	// Getters
	u32 getUID() const;
	S_Entity& getEntity() const;
	WPacket& getBuffer() const;

	// Setters
	void set_id(u32 uid);
	void setPlayer(S_Entity& player);

	// Inventory functions
	bool pickUpItem(const DropableItem& item);
	void dropItemFromInventory(const ItemStack& stack, const uint8_t& slot);
};

