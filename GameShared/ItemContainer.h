#pragma once
#include "Items.h"
#include "Loader.h"
#include "WPacket.h"
#include "RPacket.h"

template<u16 Size>
class ItemContainer
{
public:
	ItemStack itemStacks[Size];
	bool dirty;

public:
	ItemContainer()
		: dirty(true)
	{
		for (int i = 0; i < Size; i++)
			itemStacks[i] = ItemStack(ITEM_BONES, 0);
	}
	~ItemContainer()
	{

	}

	void serialize(WPacket& packet)
	{
		for (int i = 0; i < Size; i++)
			packet.write<ItemStack>(itemStacks[i]);
	}

	void deserialize(RPacket& packet)
	{
		for (int i = 0; i < Size; i++)
			itemStacks[i] = *packet.read<ItemStack>();
	}

	// Attemps to remove an item from the container.
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
				for (int i = 0; i < Size; i++)
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
				return false; // No stacks in the ItemContainer contain this unstackable item
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
					// The item is in the ItemContainer, but not in great enough quantity
					return false;
				}

			// The item was not found in the given slot
			for (int i = 0; i < Size; i++)
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

	/*bool tryRemoveItem(const ItemType type, int amount)
	{
		if (getItemCount(type) >= amount)
		{
			for (int i = 0; i < Size; i++)
				if (itemStacks[i].type == type)
				{
					if (itemStacks[i].count <= amount)
					{
						itemStacks[i].count -= amount;
						return true;
					}
					else
					{
						amount -= itemStacks[i].count;
						itemStacks[i].count = 0;
						if (amount == 0)
							return true;
					}
				}
		}
		return false;
	}*/

	bool removeOneItem(ItemType type)
	{
		for (int i = 0; i < Size; i++)
			if (itemStacks[i].type == type && itemStacks[i].count > 0)
			{
				itemStacks[i].count--;
				dirty = true;
				return true;
			}
		return false;
	}

	bool removeOneItem(ItemType type, u8 slot)
	{
		if (itemStacks[slot].type == type && itemStacks[slot].count > 0)
		{
			itemStacks[slot].count--;
			dirty = true;
			return true;
		}
		else
			return removeOneItem(type);
	}

	// Sets out_slot to the index of the first slot containing a stack of the given item type.
	// Returns true if a stack was found and false if it was not found
	bool firstSlotContaining(ItemType type, u8& out_slot) const
	{
		for (out_slot = 0; out_slot < Size; out_slot++)
			if (itemStacks[out_slot].type && itemStacks[out_slot].count > 0)
				return true;
		return false;
	}

	bool getPreferredSlot(ItemType type, u8& slot) const
	{
		if (itemStacks[slot].type == type)
			return true;
		else
			return firstSlotContaining(type, slot);
	}

	// Returns true if the container has any stacks of the given ItemType
	bool containsItemType(ItemType type) const
	{
		for (int i = 0; i < Size; i++)
			if (itemStacks[i].type == type && itemStacks[i].count > 0)
				return true;
		return false;
	}

	// Returns the amount of this type of item in the container
	u32 getItemCount(ItemType type) const
	{
		u32 count = 0;
		for (int i = 0; i < Size; i++)
			if (itemStacks[i].type == type)
				count += itemStacks[i].count;
		return count;
	}

};

typedef ItemContainer<28> Inventory;
typedef ItemContainer<256> Bank;