#pragma once
#include <SFML/Graphics.hpp>

class MenuTab
{
protected:
	const sf::FloatRect m_menuBounds;

public:
	MenuTab();
	~MenuTab();

	const sf::FloatRect& getBounds();

	virtual void onMousePressed(const sf::Mouse::Button& button, const sf::Vector2f& mousePos) = 0;
	virtual void onMouseReleased(const sf::Mouse::Button& button, const sf::Vector2f& mousePos) = 0;
	virtual void update(const sf::Vector2f& mousePos) = 0;

	virtual void draw(sf::RenderTarget& target) = 0;
};

class InventoryTab;
