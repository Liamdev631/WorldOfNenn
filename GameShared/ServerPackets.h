#pragma once
#include "Global.h"
#include "Items.h"
#include "CombatState.h"
#include "MovementState.h"

typedef u8 packet_header;

/*
This is the first game packet the client should recieve.
*/
#pragma pack(push, 1)
constexpr packet_header SP_Hello_header = 0;
struct SP_Hello
{
	const packet_header header = SP_Hello_header; // 0
	u32 timestamp;
	u16 uid;
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_EntityStatus_header = 1;
struct SP_EntityStatus 
{
	const packet_header header = 1; // 1
	u8 numEntities;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SP_EntityStatus_Elem
{
	u16 uid;
	EntityType entityType;
	MovementState move;
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_RemoveEntity_header = 2;
struct SP_RemoveEntity
{
	const packet_header header = SP_RemoveEntity_header; // 2
	u16 uid;
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_Inventory_header = 3;
struct SP_Inventory
{
	const packet_header header = SP_Inventory_header; // 3
	u16 uid;
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_EntityTookDamage_header = 4;
struct SP_EntityTookDamage
{
	const packet_header header = SP_EntityTookDamage_header; // 4
	u16 defender;
	u8 damage;
	CombatState combatState;

	SP_EntityTookDamage(const u16& defender, const u8& damage, const CombatState& combatState)
		: defender(defender), damage(damage), combatState(combatState) { }
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_ItemPicked_header = 5;
struct SP_ItemPicked
{
public:
	const packet_header header = SP_ItemPicked_header; // 5
	DropableItem item;

	SP_ItemPicked(const DropableItem& item)
		: item(item) { }
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_ItemDropped_header = 6;
struct SP_ItemDropped
{
public:
	const packet_header header = SP_ItemDropped_header; // 6
	DropableItem item;

	SP_ItemDropped(const DropableItem& item)
		: item(item) { }
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_EntityMoved_header = 7;
struct SP_EntityMoved
{
	const packet_header header = SP_EntityMoved_header; // 7
	u16 uid;
	MovementState move;
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_TimestampSync_header = 8;
struct SP_TimestampSync
{
	const packet_header header = SP_TimestampSync_header;
	u32 timestamp;
};
#pragma pack(pop)
