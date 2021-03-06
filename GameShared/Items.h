#pragma once
#include "Global.h"
#include "ItemType.h"

/// A stack of items
#pragma pack(push, 1)
struct ItemStack
{
	u32 count;
	ItemType type;

	ItemStack();
	ItemStack(const ItemType& type, const u32& count);

	const std::wstring& getItemName() const;
	const std::wstring getFormattedCountString() const;

	bool operator==(const ItemStack& rhs) const { return count == rhs.count && type == rhs.type; }
	bool operator!=(const ItemStack& rhs) const { return count != rhs.count || type != rhs.type; }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DropableItem
{
	ItemStack stack;
	vec2s pos;

	DropableItem() = default;
	DropableItem(const ItemStack& stack, const vec2s& pos);
	DropableItem(const ItemStack& stack, const u16& x, const u16& y);

	bool operator==(const DropableItem& rhs) const { return pos == rhs.pos && stack == rhs.stack; }
	bool operator!=(const DropableItem& rhs) const { return pos != rhs.pos || stack != rhs.stack; }

	const std::wstring& getItemName();
};
#pragma pack(pop)

