#pragma once
#include "S_Entity.h"
#include "Items.h"
#include "Global.h"
#include "Skills.h"
#include "ItemContainer.h"
#include "Object.h"

class S_Entity_Player : public S_Entity
{
public:
	Inventory inventory;
	ExperienceTable exp;
	char username[12];
	char password[12];
	bool forceDisconnect;

private:
	WPacket* m_buffer;

public:
	S_Entity_Player();
	~S_Entity_Player();

	// Getters
	WPacket& getBuffer() const;

	// Attemps to remove the given DropableItem from the world and add it to the player's inventory
	bool pickUpItem(DropableItem item);

	// Attempts to remove an item from the players inventory, notifying the player if it does
	void dropItemFromInventory(const ItemStack& stack, const uint8_t& slot);

	// Prints a wstring on the player's client's chatbox
	void printInChatbox(const std::wstring& string);

	// Adds experience to a skill and notifies the player of a change
	void addExperience(Skill skill, exp_val amount);

	// Sends a list of all objects close to the player's position
	void sendAllNearbyObjects() const;

	void sendObject(Object& object);

	// Overrides
	void onRespawn() override;

	void onDeath() override;
};

