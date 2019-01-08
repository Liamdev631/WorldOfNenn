#pragma once
#include "S_Entity_Player.h"
#include "Items.h"

void buryRegBones(S_Entity_Player& player, u8 slot)
{
	if (player.inventory.removeOneItem(ItemType::ITEM_BONES, slot))
	{
		player.addExperience(Skill::SK_Piety, 5);
		player.printInChatbox(L"You bury some bones.\n");
	}
}
