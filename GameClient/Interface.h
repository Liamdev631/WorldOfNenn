#pragma once
#include "SFML/Graphics.hpp"

class Interface
{
protected:
	static sf::Vector2f m_mousePos;

public:
	Interface();
	~Interface();

	virtual void update() = 0;
	virtual void draw(sf::RenderTarget& target) = 0;
	virtual void onMousePressed(sf::Mouse::Button button) = 0;
	virtual void onMouseReleased(sf::Mouse::Button button) = 0;
};

