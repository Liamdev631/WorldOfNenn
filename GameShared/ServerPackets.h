#pragma once
#include "Global.h"
#include "Items.h"
#include "CombatState.h"
#include "MoveKey.h"
#include "Skills.h"
#include "EntityType.h"
#include <string>

typedef u8 packet_header;

/*
This is the first game packet the client should recieve.
*/
#pragma pack(push, 1)
constexpr packet_header SP_Hello_header = 0;
struct SP_Hello
{
	const packet_header header = SP_Hello_header;
	u32 timestamp;
	u16 uid;
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_EntityStatus_header = 1;
struct SP_EntityStatus 
{
	const packet_header header = SP_EntityStatus_header;
	u16 uid;
	EntityType entityType;
	MoveKey move;
	char username[12];

	void setUsername(char* user) {
		strcpy_s(username, 12, user);
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_RemoveEntity_header = 2;
struct SP_RemoveEntity
{
	const packet_header header = SP_RemoveEntity_header;
	u16 uid;
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_Inventory_header = 3;
struct SP_Inventory
{
	const packet_header header = SP_Inventory_header;
	u16 uid;
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr packet_header SP_EntityTookDamage_header = 4;
struct SP_EntityTookDamage
{
	const packet_header header = SP_EntityTookDamage_header;
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
	const packet_header header = SP_ItemPicked_header;
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
	const packet_header header = SP_ItemDropped_header;
	DropableItem item;

	SP_ItemDropped(const DropableItem& item)
		: item(item) { }
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

#pragma pack(push, 1)
constexpr packet_header SP_SetRun_header = 9;
struct SP_SetRun
{
	const packet_header header = SP_SetRun_header;
	bool run;

	SP_SetRun(bool run)
		: run(run) { }
};
#pragma pack(pop)

constexpr packet_header SP_ChatText_header = 10;
#pragma pack(push, 1)
struct SP_ChatText
{
	const packet_header header = SP_ChatText_header;
	wchar_t message[64];
	u16 speaker;

	SP_ChatText(std::wstring text, u16 speaker)
		: speaker(speaker)
	{
		if (text.length() >= 64)
			return;
		memcpy_s(&message, sizeof(wchar_t) * 64, text.c_str(), 64);
		message[text.length()] = '\0';
	}
};
#pragma pack(pop)

constexpr packet_header SP_ExperienceTable_header = 11;
#pragma pack(push, 1)
struct SP_ExperienceTable
{
	const packet_header header = SP_ExperienceTable_header;
	ExperienceTable exp;

	SP_ExperienceTable(const ExperienceTable& exp)
		: exp(exp)
	{

	}
};
#pragma pack(pop)

constexpr packet_header SP_PrintMessage_header = 12;
#pragma pack(push, 1)
struct SP_PrintMessage
{
	const packet_header header = SP_PrintMessage_header;
	wchar_t message[64];

	SP_PrintMessage(std::wstring text)
	{
		if (text.length() >= 64)
			return;
		memcpy_s(&message, sizeof(wchar_t) * 64, text.c_str(), sizeof(wchar_t) * 64);
		message[text.length()] = '\0';
	}
};
#pragma pack(pop)

enum LoginResult
{
	None,
	Success,
	PasswordIncorrect,
	ServerFull,
};

constexpr packet_header SP_LoginResult_header = 13;
#pragma pack(push, 1)
struct SP_LoginResult
{
	const packet_header header = SP_LoginResult_header;
	LoginResult result;

	SP_LoginResult(LoginResult result)
		: result(result)
	{

	}
};
#pragma pack(pop)

constexpr packet_header SP_CombatStateChange_header = 14;
#pragma pack(push, 1)
struct SP_CombatStateChange
{
	const packet_header header = SP_CombatStateChange_header;
	u16 entity;
	CombatState newState;

	SP_CombatStateChange(u16 entity, CombatState newState)
		: entity(entity), newState(newState)
	{

	}
};
#pragma pack(pop)
