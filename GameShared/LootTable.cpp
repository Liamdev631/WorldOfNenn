#include "LootTable.h"
#include "LootTableValues.h"
#include <map>

namespace hidden
{
	std::map<EntityType, LootTableList> lootTableMap;
	bool initialized = false;

	// Properly adds a loot table entry.
	// ATTENTION! The LootTable passed must be 
	void addLootTable(EntityType entityType, const LootTable* table)
	{
		lootTableMap[entityType].push_back(table);
	}

	// Makes sure the given entity will always drop the given LootEntry
	void addAlwaysDrop(EntityType entityType, LootEntry loot)
	{
		assert(loot.dropRate == DR_ALWAYS);
		lootTableMap[entityType].push_back(new LootTable({ loot }));
	}

	void buildLootTables()
	{
		// 2 - Rat
		addLootTable(EntityType::ET_RAT, &Table_SingleBone);

		// 3 - Big Rat
		addLootTable(EntityType::ET_BIGRAT, &Table_SingleBone);
		addAlwaysDrop(EntityType::ET_BIGRAT, LootEntry(ItemType::ITEM_RAW_RAT_MEAT));
		addAlwaysDrop(EntityType::ET_BIGRAT, LootEntry(ItemType::ITEM_COINS, 3, 5));
	}

	LootTableList emptyList = LootTableList();
}


const LootTableList& LootGenerator::getEntityLootTables(EntityType t)
{
	if (!hidden::initialized)
	{
		hidden::buildLootTables();
		hidden::initialized = true;
	}

	auto val = hidden::lootTableMap.find(t);
	if (val == hidden::lootTableMap.end())
		return hidden::emptyList;
	return val->second;
}

std::vector<ItemStack> LootGenerator::generateLoot(EntityType entityType)
{
	std::vector<ItemStack> ret;
	for (auto& lootTable : getEntityLootTables(entityType))
	{
		u16 amount;
		if (lootTable->size() == 1)
		{
			// Only one item in the table. Add it.
			const auto& item = (*lootTable)[0];
			amount = item.amount;
			if (item.range > 0)
				amount += rand() % item.range;
			ret.push_back(ItemStack(item.type, amount));
		}
		else
		{
			u16 cumulativeChance = rand() % DR_ALWAYS;
			for (u8 i = (u8)lootTable->size() - 1; i >= 0; i--)
			{
				auto& dr = (*lootTable)[i].dropRate;

				// Entries with a lower index have the highest drop rate
				if (cumulativeChance < dr)
				{
					const auto& item = (*lootTable)[i];
					amount = item.amount;
					if (item.range > 0)
						amount += rand() % item.range;
					ret.push_back(ItemStack(item.type, amount));
					break;
				}
				else
				{
					cumulativeChance -= dr;
				}
			}
		}
	}
	return ret;
}