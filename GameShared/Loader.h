#pragma once
#include <vector>
#include "Global.h"
#include "Items.h"
#include "EntityType.h"
#include "ItemType.h"
#include "Equipment.h"
#include <map>

//constexpr size_t NumberOfItems = 4;
//constexpr size_t NumberOfEntityTypes = 4;

// Globalization
enum Language
{
	English = 0,
	ChineseSimp = 1,
};

class Loader
{
private:
	Loader();

	const Language m_language;

	// Items
	std::wstring	m_itemNames[ItemType::ITEM_COUNT];
	u32				m_itemMaxStack[ItemType::ITEM_COUNT];
	std::wstring	m_itemDescription[ItemType::ITEM_COUNT];

	// Entities
	std::wstring	m_entityNames[EntityType::ET_COUNT];
	std::wstring	m_entityDescription[EntityType::ET_COUNT];

	// Weapons
	std::map<ItemType, WeaponStats> m_weaponData;

public:
	~Loader();

	static Loader& get()
	{
		static Loader l;
		return l;
	}
	
	// Items
	const std::wstring&	getItemName(const ItemType& item) const;
	const u32&			getItemMaxStack(const ItemType& item) const;
	const std::wstring&	getItemDescription(const ItemType& item) const;
	
	// Entities
	const std::wstring& getEntityName(const EntityType type) const;
	const std::wstring& getEntityDescription(const EntityType type) const;

	// Weapons
	const WeaponStats* getWeaponData(ItemType type) const;
	bool isWeapon(ItemType type);

private:
	void load();
	void loadItemsInfo();
	void loadEntityInfo();
	void loadWeaponsInfo();
};
