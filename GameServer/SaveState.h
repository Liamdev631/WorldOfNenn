#pragma once
#include "S_Entity_Player.h"
#include "Login.h"

struct SaveState
{
public:
	char username[12];
	char password[12];
	vec2s position;
	Inventory inventory;
	ExperienceTable expTable;

private:

public:
	SaveState() = default;
	~SaveState() = default;

	static void savePlayerState(SaveState& saveState, S_Entity_Player* player);
	static void loadPlayersState(const SaveState& saveState, S_Entity_Player* player);
	static SaveState newSave(const S_Entity_Player* player);
};

