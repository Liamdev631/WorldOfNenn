#pragma once
#include "Global.h"

enum ItemType : u32
{
	ITEM_BONES = 0,
	ITEM_BRONZE_DAGGER = 1,
	ITEM_BRONZE_SHIELD = 2,
	ITEM_BRONZE_PLATEBODY = 3,
	ITEM_BRONZE_PLATELEGS = 4,
	ITEM_BRONZE_HELMET = 5,
	ITEM_BRONZE_BOOTS = 6,
	ITEM_LEATHER_BOOTS = 7,
	ITEM_LEATHER_GLOVES = 8,
	ITEM_COUNT
};

#pragma pack(push, 1)
/// A stack of items
struct ItemStack
{
	u32 count;
	ItemType type;

	ItemStack()
		: type(ITEM_BONES), count(0) { }
	ItemStack(const ItemType& type, const u32& count)
		: type(type), count(count) { }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DropableItem
{
	ItemStack stack;
	vec2s pos;

	DropableItem() = default;
	DropableItem(const ItemStack& stack, const vec2s& pos)
		: stack(stack), pos(pos) { }
	DropableItem(const ItemStack& stack, const u16& x, const u16& y)
		: stack(stack), pos(vec2s(x, y)) { }

	bool operator==(const DropableItem& rhs) { return pos == rhs.pos && stack.type == rhs.stack.type; }
	bool operator!=(const DropableItem& rhs) { return pos == rhs.pos || stack.type != rhs.stack.type; }
};
#pragma pack(pop)

