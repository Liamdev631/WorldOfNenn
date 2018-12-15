#include "S_Connection.h"
#include "Loader.h"
#include "ServerPackets.h"
#include "S_Server.h"

S_Connection::S_Connection()
	: m_buffer(new WPacket(512)), m_player(nullptr)
{

}

S_Connection::~S_Connection()
{

}

u32 S_Connection::getUID() const
{
	return m_uid;
}

void S_Connection::set_id(u32 uid)
{
	m_uid = uid;
}

S_Entity& S_Connection::getEntity() const
{
	return *m_player;
}

void S_Connection::setPlayer(S_Entity& player)
{
	if (!m_player)
		m_player = &player;
}

WPacket& S_Connection::getBuffer() const
{
	return *m_buffer;
}

bool S_Connection::pickUpItem(const DropableItem& item)
{
	auto& region = m_player->getMovement().getWorldRegion();

	bool taken = false;

	// First, try to take the item from the world
	for (auto iter = region.getItems().begin(); iter != region.getItems().end(); iter++)
		if ((*iter).stack.type == item.stack.type)
		{
			// If the item was taken, find a suitable slot
			ItemStack newItem = item.stack;
			u32 maxStack = Loader::get().getItemMaxStack(newItem.type);
			if (maxStack == 1)
			{
				// Not stackable
				for (int i = 0; i < 28; i++)
					if (inventory.itemStacks[i].count == 0)
					{
						inventory.itemStacks[i] = newItem;
						inventory.itemStacks[i].count = 1;
						inventory.dirty = true;
						taken = true;
						break;
					}
			}
			else
			{
				for (int i = 0; i < 28; i++)
				{
					if (inventory.itemStacks[i].type == newItem.type)
					{
						if (inventory.itemStacks[i].count + newItem.count >= maxStack)
						{
							// Not enough room in this stack, but we can take some
							int amountToRemove = maxStack - inventory.itemStacks[i].count;
							newItem.count -= amountToRemove;
							inventory.itemStacks[i].count = maxStack;
							inventory.dirty = true;
							taken = true;
						}
						else
						{
							inventory.itemStacks[i].count += newItem.count;
							inventory.dirty = true;
							taken = true;
						}
					}
				}
			}

			if (taken)
			{
				// Remove the item from the world
				region.removeGroundItem(iter);

				//region.getItems().erase(iter);

				//// Tell the players
				//for (auto iter = region.getConnections().begin(); iter != region.getConnections().end(); iter++)
				//{
				//	WPacket& buffer = iter->second->getBuffer();
				//	buffer.write<u8>(0x05);
				//	buffer.write<DropableItem>(item);
				//}
			}

			break;
		}

	return taken;
}

void S_Connection::dropItemFromInventory(const ItemStack& stack, const uint8_t& slot)
{
	u8 slotRemovedFrom = slot;
	if (!inventory.tryRemoveItem(stack, slotRemovedFrom)) {
		return; // If the item wasn't removed, just return
	}


	// Place the item stack in the world
	auto& currentRegion = m_player->getMovement().getWorldRegion();
	auto& playerPos = m_player->getMovement().position;
	currentRegion.addGroundItem(DropableItem(stack, playerPos.x, playerPos.y));
}
