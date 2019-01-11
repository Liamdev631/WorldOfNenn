#pragma once
#include "UIComponent.h"

class MenuTab : public UIComponent
{
protected:
	const sf::FloatRect m_menuBounds;

public:
	MenuTab();
	~MenuTab();

	const sf::FloatRect& getGlobalBounds();

	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override = 0;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override = 0;
	void draw(sf::RenderTarget& target) const override = 0;
};
