#pragma once
#include "Global.h"
#include "Items.h"

typedef u8 packet_header;

#pragma pack(push, 1)
#define CP_AttackEntity_header 0
struct CP_AttackEntity
{
public:
	const packet_header header = CP_AttackEntity_header;
	u16 uid;

	CP_AttackEntity(const u16& target)
		: uid(target) { }
};
#pragma pack(pop)

#pragma pack(push, 1)
#define CP_ItemPicked_header 1
struct CP_ItemPicked
{
public:
	const packet_header header = CP_ItemPicked_header;
	DropableItem item;

	CP_ItemPicked(const DropableItem& item)
		: item(item) { }
};
#pragma pack(pop)

#pragma pack(push, 1)
#define CP_ItemDropped_header 2
struct CP_ItemDropped
{
public:
	const packet_header header = CP_ItemDropped_header;
	ItemStack item;
	uint8_t slot;

	CP_ItemDropped(const ItemStack& item, const uint8_t& slot)
		: item(item), slot(slot) { }
};
#pragma pack(pop)

#pragma pack(push, 1)
#define CP_MoveToDest_header 3
struct CP_MoveToDest
{
	const packet_header header = CP_MoveToDest_header;
	vec2<u16> pos;

	CP_MoveToDest(vec2s pos)
		: pos(pos) { }
};
#pragma pack(pop)

#pragma pack(push, 1)
#define CP_SetRun_header 4
struct CP_SetRun
{
	const packet_header header = CP_SetRun_header;
	bool run;

	CP_SetRun(bool run)
		: run(run) { }
};
#pragma pack(pop)
