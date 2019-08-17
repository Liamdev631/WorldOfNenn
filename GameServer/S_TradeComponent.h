#pragma once
#include "Types.h"
#include "ItemContainer.h"

class S_Entity_Player;

class S_TradeComponent
{
public:
	S_Entity_Player& owner;
	u16 tradePartner;
	Inventory offer;
	bool isActive;

public:
	S_TradeComponent(S_Entity_Player& owner);
	~S_TradeComponent();

	void openTrade(u16 tradeParter);
};

