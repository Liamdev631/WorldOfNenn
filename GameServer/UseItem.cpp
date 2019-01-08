#include "UseItem.h"
#include "UseItemFunctions.h"
#include <functional>
#include <map>

typedef std::function<void(S_Entity_Player&, u8)> useItemFunc;
std::map<ItemType, useItemFunc> funcHashes;

UseItem::UseItem()
{
}

UseItem::~UseItem()
{

}

void UseItem::init()
{
	funcHashes[ITEM_BONES] = buryRegBones;
}

void UseItem::use(S_Entity_Player& player, ItemType itemType, u8 slot)
{
	auto& inventory = player.inventory;

	// Find the slot of the item to use
	//u8 trueSlot = 0;
	//if (inventory.itemStacks[slot].type == itemType)
	//	trueSlot = slot;
	//else
	//	if (!inventory.firstSlotContaining(itemType, trueSlot))
	//		return; // Inventory doesn't have the item

	// Find the items use function
	auto func = funcHashes.find(itemType);
	if (func == funcHashes.end())
	{
		printf("Warning! ItemType %u has no use function!\n", itemType);
		return; // There is no use function for this item
	}

	// Call the use function
	func->second(player, slot);
}
