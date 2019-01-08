#pragma once
#include "S_Entity_Player.h"
#include "Items.h"

class UseItem
{
private:
	UseItem();

public:
	~UseItem();

	// Initializes the function table. Must be run before any items are used.
	static void init();

	// Attempts to perform the use action of the given item. The function tries to use
	// the item located in the given slot and uses the first occurance of the item type
	// the item is not found in that slot.
	static void use(S_Entity_Player& player, ItemType itemType, u8 slot);
};

