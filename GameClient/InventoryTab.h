#pragma once
#include "MenuTab.h"
#include "Items.h"

class InventoryTab : public MenuTab
{
protected:
	sf::RectangleShape m_itemSlotImages[28];

private:

public:
	InventoryTab();
	~InventoryTab();

	void setHighlightedSlot(const int& slotclicked);

	// Returns which slot of the inventory is being pointed at by the mouse, or -1 if none are
	int getSlotUnderMouse(const sf::Vector2f& mousePos) const;

	// UIComponent override
	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void openRCOMenuForSlot(const int& slotClicked);
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const override;
};

