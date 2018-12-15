#include "Crosshair.h"

Crosshair::Crosshair()
	: m_size(16, 16), m_thickness(1), m_color(sf::Color::Black)
{
	calcShape();
}

Crosshair::~Crosshair()
{

}

void Crosshair::calcShape()
{
	m_shape.setFillColor(sf::Color::Transparent);
	m_shape.setOutlineColor(m_color);
	m_shape.setOutlineThickness(m_thickness);
	m_shape.setSize(m_size);
}

void Crosshair::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape);
}

void Crosshair::setPosition(const sf::Vector2f& position)
{
	m_shape.setPosition(position);
}
