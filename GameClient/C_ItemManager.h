#pragma once
#include "Global.h"
#include <vector>
#include "Items.h"

class C_ItemManager
{
private:
	std::vector<DropableItem> m_items;

public:
	~C_ItemManager();

	static C_ItemManager& get()
	{
		static C_ItemManager i;
		return i;
	}

	void pickUpItem(const DropableItem& item);
	void dropItemFromInventory(const DropableItem& item);
	std::vector<DropableItem>& getItemsList();

private:
	C_ItemManager();
};

