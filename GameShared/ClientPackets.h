#pragma once
#include "Global.h"
#include "Items.h"
#include <string>
#include "EntityType.h"
#include "Login.h"

typedef u8 packet_header;

#define CP_AttackEntity_header 0
#pragma pack(push, 1)
struct CP_AttackEntity
{
public:
	const packet_header header = CP_AttackEntity_header;
	u16 uid;

	CP_AttackEntity(const u16& target)
		: uid(target) { }
};
#pragma pack(pop)

#define CP_ItemPicked_header 1
#pragma pack(push, 1)
struct CP_ItemPicked
{
public:
	const packet_header header = CP_ItemPicked_header;
	DropableItem item;

	CP_ItemPicked(const DropableItem& item)
		: item(item) { }
};
#pragma pack(pop)

#define CP_ItemDropped_header 2
#pragma pack(push, 1)
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

#define CP_MoveToDest_header 3
#pragma pack(push, 1)
struct CP_MoveToDest
{
	const packet_header header = CP_MoveToDest_header;
	vec2<u16> pos;

	CP_MoveToDest(vec2s pos)
		: pos(pos) { }
};
#pragma pack(pop)

#define CP_SetRun_header 4
#pragma pack(push, 1)
struct CP_SetRun
{
	const packet_header header = CP_SetRun_header;
	bool run;

	CP_SetRun(bool run)
		: run(run) { }
};
#pragma pack(pop)

#define CP_ChatText_header 5
#pragma pack(push, 1)
struct CP_ChatText
{
	const packet_header header = CP_ChatText_header;
	wchar_t message[64];

	CP_ChatText(std::wstring text)
	{
		if (text.length() >= 64)
			return;
		memcpy_s(&message, sizeof(wchar_t) * 64, text.c_str(), sizeof(wchar_t) * 64);
		message[text.length()] = '\0';
	}
};
#pragma pack(pop)

#define CP_UseItem_header 6
#pragma pack(push, 1)
struct CP_UseItem
{
	const packet_header header = CP_UseItem_header;
	ItemType itemType;
	u8 slot;

	CP_UseItem(ItemType itemType, u8 slot)
		: itemType(itemType), slot(slot) { }
};
#pragma pack(pop)

#define CP_Login_header 7
#pragma pack(push, 1)
struct CP_Login
{
	const packet_header header = CP_Login_header;
	char username[12];
	char password[12];
	u16 buildNumber;

	CP_Login(const std::string& user, const std::string& pass, u16 build)
	{
		buildNumber = build;
		//if (user.length() <= 11)
		//{
		//	//auto size = sizeof(char) * NAME_LENGTH;
		//	//memcpy_s(&username, size, user.c_str(), size);
		//	//username[user.length()] = '\0';
		//}

		//if (pass.length() <= 11)
		//{
		//	//auto size = sizeof(char) * NAME_LENGTH;
		//	//memcpy_s(&password, size, pass.c_str(), size);
		//	//username[pass.length()] = '\0';
		//}
		strcpy_s(username, 12, user.c_str());
		strcpy_s(password, 12, pass.c_str());
	}

	std::string getUsername() const
	{
		return std::string(username);
	}

	std::string getPassword() const
	{
		return std::string(password);
	}
};
#pragma pack(pop)
