#include "C_ItemManager.h"

C_ItemManager::C_ItemManager()
{

}

C_ItemManager::~C_ItemManager()
{

}

void C_ItemManager::dropItemFromInventory(const DropableItem& item)
{
	m_items.push_back(item);
}

void C_ItemManager::pickUpItem(const DropableItem& item)
{
	for (auto iter = m_items.begin(); iter != m_items.end(); iter++)
		if ((*iter) == item)
		{
			m_items.erase(iter);
			break;
		}
}

std::vector<DropableItem>& C_ItemManager::getItemsList()
{
	return m_items;
}

