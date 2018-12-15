#pragma once
#include "SFML/Graphics.hpp"

class Healthbar : public sf::Drawable
{
private:
	sf::RectangleShape m_background;
	sf::RectangleShape m_foreground;
	sf::Vector2f m_topLeft;

public:
	Healthbar();
	~Healthbar();

	void setSize(int width, int height);
	void setCenter(const sf::Vector2f& center);
	void setColors(const sf::Color& background, const sf::Color& foreground);
	void setValue(const float value);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
