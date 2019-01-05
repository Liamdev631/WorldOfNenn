#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceLoader.h"
#include "GameTime.h"

class UIComponent
{
protected:
	std::vector<UIComponent*> m_children;

public:
	~UIComponent()
	{
		for (auto& c : m_children)
			delete c;
	}
	//virtual void onMousePressed(const sf::Mouse::Button& button, const sf::Vector2f& mousePos) = 0;
	//virtual void onMouseReleased(const sf::Mouse::Button& button, const sf::Vector2f& mousePos) = 0;
	//virtual void onKeyPressed(const sf::Keyboard::Key& key) = 0;
	virtual void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) = 0;
	virtual void update(const GameTime& time, const sf::Vector2f& mousePos) = 0;
	virtual void draw(sf::RenderTarget& target) const = 0;
};
