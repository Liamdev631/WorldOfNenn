#include "Items.h"
#include "Loader.h"

ItemStack::ItemStack()
	: type(ITEM_BONES), count(0)
{
}
ItemStack::ItemStack(const ItemType& type, const u32& count)
	: type(type), count(count)
{
}

const std::wstring& ItemStack::getItemName() const
{
	return Loader::get().getItemName(type);
}

const std::wstring ItemStack::getFormattedCountString() const
{
	u32 c = count;
	wchar_t symbol = L' ';
	if (count > 1000000000)
	{
		symbol = L'M';
		c /= 1000000;
	}
	else if (count > 1000000)
	{
		symbol = L'K';
		c /= 1000;
	}

	return std::to_wstring(c) + symbol;
}

DropableItem::DropableItem(const ItemStack& stack, const vec2s& pos)
	: stack(stack), pos(pos)
{
}

DropableItem::DropableItem(const ItemStack& stack, const u16& x, const u16& y)
	: stack(stack), pos(vec2s(x, y))
{
}

const std::wstring& DropableItem::getItemName()
{
	return stack.getItemName();
}
