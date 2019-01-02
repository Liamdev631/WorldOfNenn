#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceLoader.h"

class UIComponent
{
public:
	//virtual void onMousePressed(const sf::Mouse::Button& button, const sf::Vector2f& mousePos) = 0;
	//virtual void onMouseReleased(const sf::Mouse::Button& button, const sf::Vector2f& mousePos) = 0;
	//virtual void onKeyPressed(const sf::Keyboard::Key& key) = 0;
	virtual void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) = 0;
	virtual void update(const sf::Vector2f& mousePos) = 0;
	virtual void draw(sf::RenderTarget& target) = 0;
};
