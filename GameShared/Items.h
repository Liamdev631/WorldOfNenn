#pragma once
#include "Loader.h"
#include "WPacket.h"
#include "RPacket.h"

#pragma pack(push, 1)
/// A stack of items
struct ItemStack
{
	u32 count;
	ItemType type;

	ItemStack()
		: type(ITEM_BONES), count(0) { }
	ItemStack(const ItemType& type, const u32& count)
		: type(type), count(count) { }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DropableItem
{
	ItemStack stack;
	vec2s pos;

	DropableItem() = default;
	DropableItem(const ItemStack& stack, const vec2s& pos)
		: stack(stack), pos(pos) { }
	DropableItem(const ItemStack& stack, const u16& x, const u16& y)
		: stack(stack), pos(vec2s(x, y)) { }

	bool operator==(const DropableItem& rhs) { return pos == rhs.pos && stack.type == rhs.stack.type; }
	bool operator!=(const DropableItem& rhs) { return pos == rhs.pos || stack.type != rhs.stack.type; }
};
#pragma pack(pop)

class Inventory
{
public:
	ItemStack itemStacks[28];
	bool dirty;

public:
	Inventory()
		: dirty(true)
	{
		for (int i = 0; i < 28; i++)
			itemStacks[i] = ItemStack(ITEM_BONES, 0);
	}
	~Inventory()
	{

	}

	void serialize(WPacket& packet)
	{
		for (int i = 0; i < 28; i++)
			packet.write<ItemStack>(itemStacks[i]);
	}

	void deserialize(RPacket& packet)
	{
		for (int i = 0; i < 28; i++)
			itemStacks[i] = *packet.read<ItemStack>();
	}

	// Attemps to remove an item from the inventory.
	// @itemToRemove: A stack describing the type and quantity of the item to remove.
	// @slotRemoveFrom: The slot that the stack should be removed from. This will be set to the slot that the item was removed from. If it couldnt be removed, this will be unchanged.
	// @returns: true if the item was removed, false if not.
	bool tryRemoveItem(const ItemStack& itemToRemove, u8& slotRemovedFrom)
	{
		u32 maxStack = Loader::get().getItemMaxStack(itemToRemove.type);
		if (maxStack == 1)
		{
			// Unstackable

			if (itemStacks[slotRemovedFrom].type == itemToRemove.type)
			{
				itemStacks[slotRemovedFrom].count = 0;
				dirty = true;
				return true;
			}
			else
			{
				// Remove the first stack with the correct type 
				for (int i = 0; i < 28; i++)
				{
					if (i == slotRemovedFrom)
						continue; // Already checked this one
					if (itemStacks[i].type == itemToRemove.type)
					{
						itemStacks[i].count = 0;
						slotRemovedFrom = i;
						dirty = true;
						return true;
					}
				}
				return false; // No stacks in the inventory contain this unstackable item
			}
		}
		else
		{
			// Stackable

			if (itemStacks[slotRemovedFrom].type == itemToRemove.type)
				if (itemStacks[slotRemovedFrom].count >= itemToRemove.count)
				{
					itemStacks[slotRemovedFrom].count -= itemToRemove.count;
					dirty = true;
					return true;
				}
				else
				{
					// The item is in the inventory, but not in great enough quantity
					return false;
				}

			// The item was not found in the given slot
			for (int i = 0; i < 28; i++)
			{
				if (i == slotRemovedFrom)
					continue; // Already checked this slot
				if (itemStacks[i].type == itemToRemove.type)
				{
					if (itemStacks[i].count >= itemToRemove.count)
					{
						itemStacks[slotRemovedFrom].count -= itemToRemove.count;
						dirty = true;
						return true;
					}
				}
			}
			return false;
		}
	}
};
