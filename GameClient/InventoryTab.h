#pragma once
#include "MenuTab.h"
#include "ClickableItemGrid.h"

class InventoryTab : public MenuTab
{
private:
	ClickableItemGrid m_grid;

public:
	InventoryTab();
	~InventoryTab();

	void setHighlightedSlot(const int& slotclicked);
	void openRCOMenuForSlot(const int& slotClicked);
	
	// UIComponent override
	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const override;
};

