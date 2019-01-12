#pragma once
#include <vector>
#include "Global.h"
#include "Items.h"
#include "EntityType.h"

// Common drop rates
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

struct LootEntry
{
	// The item to become the loot
	ItemType type;

	// This is the chance of the item being dropped over 8192.
	// For example, a chance of 4000 has roughly a 50% chance of being dropped.
	u16 dropRate;

	// The bounds for the quantity to be dropped.
	// The maximum quantity is amount + range.
	u16 amount;
	u16 range;

	LootEntry(ItemType type, u16 dropRate, u16 amount, u16 range)
		: type(type), dropRate(dropRate), amount(amount), range(range)
	{ }

	LootEntry(ItemType type, u16 amount, u16 range)
		: type(type), dropRate(DR_ALWAYS), amount(amount), range(range)
	{ }

	LootEntry(ItemType type)
		: type(type), dropRate(DR_ALWAYS), amount(1), range(0)
	{ }
};

typedef const std::vector<LootEntry> LootTable;
typedef std::vector<LootTable*> LootTableList;

class LootGenerator
{
public:
	static const LootTableList& getEntityLootTables(EntityType t);
	static std::vector<ItemStack> generateLoot(EntityType entityType);
};
