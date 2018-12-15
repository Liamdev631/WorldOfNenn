#pragma once
#include <SFML/Graphics.hpp>

class Crosshair : public sf::Drawable
{
private:
	sf::Vector2f m_size;
	float m_thickness;
	sf::Color m_color;

public:
	Crosshair();
	~Crosshair();

	sf::RectangleShape m_shape;

private:
	void calcShape();

public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setPosition(const sf::Vector2f& position);
};

