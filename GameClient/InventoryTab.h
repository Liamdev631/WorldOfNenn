#pragma once
#include "MenuTab.h"
#include "Items.h"

class InventoryTab : public MenuTab
{
protected:
	sf::RectangleShape m_tabs[28];

private:
	void drawItemStack(sf::RenderTarget & target, const ItemStack& item, const int& slot);

public:
	InventoryTab();
	~InventoryTab();

	void onMousePressed(const sf::Mouse::Button& button, const sf::Vector2f& mousePos) override;
	void onMouseReleased(const sf::Mouse::Button& button, const sf::Vector2f& mousePos) override;
	void update(const sf::Vector2f& mousePos) override;

	// Returns which slot of the inventory is being pointed at by the mouse, or -1 if none are
	int getSlotUnderMouse(const sf::Vector2f& mousePos) const;

	void draw(sf::RenderTarget& target) override;
};

