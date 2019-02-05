#pragma once
#include "S_Entity_Player.h"
#include "Items.h"

void buryRegBones(S_Entity_Player& player, ItemType type, u8 slot)
{
	if (player.inventory.removeOneItem(ItemType::ITEM_BONES, slot))
	{
		player.addExperience(Skill::SK_Piety, 5);
		player.printInChatbox(L"You bury some bones.\n");
	}
}

inline void eatFood(S_Entity_Player& player, ItemType type, u8 slot, u8 amount)
{
	if (!player.inventory.getPreferredSlot(type, slot))
		return; // No such food
	if (player.getCombat().heal(amount))
	{
		player.inventory.removeOneItem(type, slot);

		switch (type)
		{
			case ITEM_RAW_RAT_MEAT:
				player.printInChatbox(L"You eat the raw meat. It heals a little. Disgusting!");
				break;
			case ITEM_COOKED_RAT_MEAT:
				player.printInChatbox(L"You eat the rat meat. It heals some health.");
				break;
		}
	}
}

// Attemps to eat food that heals for 1 hp
void eatFood_1(S_Entity_Player& player, ItemType type, u8 slot)
{
	eatFood(player, type, slot, 1);
}

// Attemps to eat food that heals for 3 hp
void eatFood_3(S_Entity_Player& player, ItemType type, u8 slot)
{
	eatFood(player, type, slot, 3);
}
