#include "SaveState.h"

void SaveState::savePlayerState(SaveState& saveState, S_Entity_Player* player)
{
	strcpy_s(saveState.username, 12, player->username);
	strcpy_s(saveState.password, 12, player->password);
	saveState.inventory = player->inventory;
	saveState.expTable = player->exp;
	saveState.position = player->getMovement().getPos();
}

void SaveState::loadPlayersState(const SaveState& saveState, S_Entity_Player* player)
{
	player->inventory = saveState.inventory;
	player->exp = saveState.expTable;
	player->getMovement().blinkTo(saveState.position);
}

SaveState SaveState::newSave(const S_Entity_Player* player)
{
	Inventory StartingInventory;
	StartingInventory.itemStacks[0].type = ItemType::ITEM_COINS;
	StartingInventory.itemStacks[0].count = 100;

	ExperienceTable StartingExperience;
	for (int i = 0; i < SK_Count; i++)
		StartingExperience.setExp((Skill)i, 1);

	SaveState s;
	strcpy_s(s.username, 12, player->username);
	strcpy_s(s.password, 12, player->password);
	s.inventory = StartingInventory;
	s.expTable = StartingExperience;
	s.inventory.dirty = true;
	s.position = vec2s(115, 115);
	return s;
}
