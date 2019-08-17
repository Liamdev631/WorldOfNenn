#include "S_TradeComponent.h"
#include "S_GlobalServer.h"

S_TradeComponent::S_TradeComponent(S_Entity_Player& owner)
	: owner(owner), isActive(false)
{

}

S_TradeComponent::~S_TradeComponent()
{

}

void S_TradeComponent::openTrade(u16 target)
{
	auto partnerEntity = g_server->getWorldManager().getEntity(target);
	if (!partnerEntity || !partnerEntity->isPlayer())
		return;
	auto& partner = *partnerEntity->asPlayer();
	if (partner.getTrade().isActive || partner.getCombat().isInCombat)
	{
		owner.printInChatbox(L"That player is busy.");
		return;
	}
	else if (owner.getCombat().isInCombat)
	{
		owner.printInChatbox(L"You cannot trade while in combat.");
		return;
	}

	// Initiate trade
	tradePartner = target;
	offer = Inventory();
	isActive = true;
	owner.getMovement().stop();

	auto& partnerTrade = partner.getTrade();
	partnerTrade.tradePartner = target;
	partnerTrade.offer = Inventory();
	partnerTrade.isActive = true;
	partner.getMovement().stop();
}
