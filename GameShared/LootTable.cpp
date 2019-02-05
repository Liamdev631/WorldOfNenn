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
		/*

		How to build loot tables!

		If you want to add an item that always drops, use addAlwaysDrop().

		If you want varying drops, add a drop table. Drop tables have a list of items and their
		probabilities of being dropped. Only one item from the drop table is selected. Make sure
		add the drops with a higher drop rate first. The arguments for LootEntry are:

			LootEntry(item type, drop rate, quantity, range)

		The quantity of loot dropped is random from quantity to quantity + range.
		If you want to drop between 10 and 15 gold you can use:

			LootEntry(ItemType::IT_COINS, DR_QUARTER, 10, 5)

		You can use these values for drop rates or pick your own by hand
		#define DR_ALWAYS 8192
		#define DR_HALF 4096
		#define DR_QUARTER 2048
		#define DR_EIGTH 1024
		#define DR_COMMON (DR_ALWAYS / 16)
		#define DR_UNCOMMON (DR_ALWAYS / 32)
		#define DR_RARE (DR_ALWAYS / 128)
		#define DR_SUPER_RARE (DR_ALWAYS / 256)
		#define DR_MEGA_RARE (DR_ALWAYS / 2048)
		#define DR_MIN 1

		For example, of you want a rat to have a 50% chance to drop bones and a 25% chance to drop meat, the code
		would look like this...
		addNewLootTable(EntityType::ET_RAT, new LootTable({
			LootEntry(ItemType::ITEM_BONES, DR_HALF, 1, 0),
			LootEntry(ItemType::ITEM_RAW_RAT_MEAT, DR_QUARTER, 1, 0),
		));

		There also premade loot tables and loot entries in LootTableValues.h that you can use and add to.

		*/

		// 2 - Rat
		addAlwaysDrop(EntityType::ET_RAT, Entry_SingleBone); // This is an example of a premade loot entry
		addAlwaysDrop(EntityType::ET_RAT, LootEntry(ItemType::ITEM_RAW_RAT_MEAT));
		addAlwaysDrop(EntityType::ET_RAT, LootEntry(ItemType::ITEM_COINS, 1, 3));

		// 3 - Pig
		addAlwaysDrop(EntityType::ET_PIG, Entry_SingleBone);
		addAlwaysDrop(EntityType::ET_PIG, LootEntry(ItemType::ITEM_COINS, 2, 5));
		addLootTable(EntityType::ET_PIG, new LootTable {
			LootEntry(ItemType::ITEM_BRONZE_SWORD, DR_COMMON, 1, 1),
			});
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
		if (lootTable->size() == 1 && lootTable->at(0).dropRate == DR_ALWAYS)
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
			u16 random = rand() % DR_ALWAYS, counter = 0;
			for (int i = lootTable->size() - 1; i >= 0; i--)
			{
				counter += (*lootTable)[i].dropRate;

				// Entries with a lower index have the highest drop rate
				if (counter > random)
				{
					const auto& item = (*lootTable)[i];
					amount = item.amount;
					if (item.range > 0)
						amount += rand() % item.range;
					ret.push_back(ItemStack(item.type, amount));
					break;
				}
			}
		}
	}
	return ret;
}