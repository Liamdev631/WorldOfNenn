#pragma once
#include <vector>
#include "Global.h"
#include "Items.h"

constexpr size_t NumberOfItems = 7;
constexpr size_t NumberOfEntityTypes = 4;

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
	std::wstring	m_itemNames[NumberOfItems];
	u32				m_itemMaxStack[NumberOfItems];
	std::wstring	m_itemDescription[NumberOfItems];

	// Entities
	std::wstring	m_entityNames[NumberOfEntityTypes];
	std::wstring	m_entityDescription[NumberOfEntityTypes];

public:
	~Loader();

	static Loader& get()
	{
		static Loader l;
		return l;
	}
	
	const std::wstring&	getItemName(const ItemType& item) const;
	const u32&			getItemMaxStack(const ItemType& item) const;
	const std::wstring&	getItemDescription(const ItemType& item) const;
	const std::wstring& getEntityName(const EntityType type) const;
	const std::wstring& getEntityDescription(const EntityType type) const;

private:
	void load();
	void loadItemsInfo();
	void loadEntityInfo();
};
