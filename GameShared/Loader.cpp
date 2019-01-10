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
}

void Loader::loadItemsInfo()
{
	CSVReader reader;
	reader.open("assets/data/ItemData.csv");
	reader.readNextRow(); // Skip header
	for (int i = 0; i < NumberOfItems; i++)
	{
		m_itemNames[i] = reader[1];
		m_itemMaxStack[i] = std::stoi(reader[2]);
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
	for (int i = 0; i < 4; i++)
	{
		m_entityNames[i] = reader[1 + (int)m_language * 2];
		m_entityDescription[i] = reader[2 + (int)m_language * 2];
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
