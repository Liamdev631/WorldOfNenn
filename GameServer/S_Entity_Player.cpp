#include "S_Entity_Player.h"
#include "S_Region.h"
#include "ServerPackets.h"

S_Entity_Player::S_Entity_Player()
	: S_Entity(9999, EntityType::ET_PLAYER, R_Overworld), m_buffer(new WPacket(1024 * 16))
{

}

S_Entity_Player::~S_Entity_Player()
{

}

WPacket& S_Entity_Player::getBuffer() const
{
	return *m_buffer;
}

bool S_Entity_Player::pickUpItem(const DropableItem& item)
{
	auto& region = getMovement().getWorldRegion();

	bool taken = false;

	// First, try to take the item from the world
	for (auto iter = region.getItems().begin(); iter != region.getItems().end(); iter++)
		if (iter->stack.type == item.stack.type && iter->pos == item.pos)
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

void S_Entity_Player::dropItemFromInventory(const ItemStack& stack, const uint8_t& slot)
{
	u8 slotRemovedFrom = slot;
	if (!inventory.tryRemoveItem(stack, slotRemovedFrom)) {
		return; // If the item wasn't removed, just return
	}

	// Place the item stack in the world
	auto& currentRegion = getMovement().getWorldRegion();
	auto& playerPos = getMovement().getPos();
	currentRegion.addGroundItem(DropableItem(stack, playerPos.x, playerPos.y));
}

void S_Entity_Player::printInChatbox(const std::wstring& message)
{
	m_buffer->write(SP_PrintMessage(message));
}

void S_Entity_Player::addExperience(Skill skill, exp_val amount)
{
	exp.addExp(skill, amount);

	// Send the player their now exp table
	m_buffer->write(SP_ExperienceTable(exp));
}

void S_Entity_Player::onDeath()
{
	S_Entity::onDeath();
	printInChatbox(L"Oh no! You have died!");
}
