#include "Loader.h"
#include "CSVReader.h"
#include <cstdint>

Loader::Loader()
	: m_language(Language::English)
{
	load();
}

Loader::~Loader()
{

}

void Loader::load()
{
	loadItemsInfo();
	loadEntityInfo();
	loadWeaponsInfo();
}

void Loader::loadItemsInfo()
{
	CSVReader reader;
	reader.open("assets/data/ItemData.csv");
	reader.readNextRow(); // Skip header
	int rows = 0;
	for (int i = 0; i < ItemType::ITEM_COUNT; i++)
	{
		rows++;
		if (reader.size() == 1)
		{
			printf("Error! ItemData row %i is missing! Should have %u rows!\n", rows, ItemType::ITEM_COUNT);
			return;
		}

		m_itemNames[i] = reader[2];
		m_itemMaxStack[i] = std::stoi(reader[1]);
		if (m_itemMaxStack[i] == 0)
			m_itemMaxStack[i] = 1000000;
		m_itemDescription[i] = reader[3];
		reader.readNextRow();
	}
}

void Loader::loadEntityInfo()
{
	CSVReader reader;
	reader.open("assets/data/EntityData.csv");
	reader.readNextRow(); // Skip header
	int rows = 0;
	for (int i = 0; i < EntityType::ET_COUNT; i++)
	{
		rows++;
		if (reader.size() == 1)
		{
			printf("Error! EntityData row %i is missing! Should have %u rows!\n", rows, EntityType::ET_COUNT);
			return;
		}
		m_entityNames[i] = reader[1 + (int)m_language * 2];
		m_entityDescription[i] = reader[2 + (int)m_language * 2];
		reader.readNextRow();
	}
}

void Loader::loadWeaponsInfo()
{
	CSVReader reader;
	reader.open("assets/data/WeaponData.csv");
	reader.readNextRow();
	while (reader.size() > 1)
	{
		auto& weaponData = m_weaponData[(ItemType)std::stoi(reader[0])];
		weaponData.stab = (u8)std::stoi(reader[1]);
		weaponData.slash = (u8)std::stoi(reader[2]);
		weaponData.crush = (u8)std::stoi(reader[3]);
		weaponData.fire = (u8)std::stoi(reader[4]);
		weaponData.frost = (u8)std::stoi(reader[5]);
		weaponData.shock = (u8)std::stoi(reader[6]);
		weaponData.special = (u8)std::stoi(reader[7]);
		reader.readNextRow();
	}
}

const std::wstring& Loader::getItemName(const ItemType& item) const
{
	return m_itemNames[item];
}

const u32& Loader::getItemMaxStack(const ItemType& item) const
{
	return m_itemMaxStack[item];
}

const std::wstring& Loader::getItemDescription(const ItemType& item) const
{
	return m_itemDescription[item];
}

const std::wstring& Loader::getEntityName(const EntityType type) const
{
	return m_entityNames[type];
}

const std::wstring& Loader::getEntityDescription(const EntityType type) const
{
	return m_entityDescription[type];
}

const WeaponStats* Loader::getWeaponData(ItemType type) const
{
	auto weapon = m_weaponData.find(type);
	if (weapon == m_weaponData.end())
		return nullptr;
	return &weapon->second;
}

bool Loader::isWeapon(ItemType type)
{
	for (auto weapon = m_weaponData.begin(); weapon != m_weaponData.end(); weapon++)
		if (weapon->first == type)
			return true;
	return false;
}
