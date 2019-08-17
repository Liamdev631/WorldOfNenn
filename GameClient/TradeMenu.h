#pragma once
#include "UIComponent.h"

class TradeMenu : UIComponent
{
private:
	sf::RectangleShape m_border;

public:
	bool isOpen;

public:
	TradeMenu();
	~TradeMenu();

	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const override;
};

