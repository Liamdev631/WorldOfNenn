#include "Healthbar.h"


Healthbar::Healthbar()
{
	m_background.setOutlineColor(sf::Color::Black);
	m_background.setOutlineThickness(1);
	setColors(sf::Color::Red, sf::Color::Green);
}

Healthbar::~Healthbar()
{

}

void Healthbar::setSize(int width, int height)
{
	m_background.setSize(sf::Vector2f((float)width, (float)height));
	m_foreground.setSize(sf::Vector2f((float)width, (float)height));
}

void Healthbar::setCenter(const sf::Vector2f& center)
{
	m_topLeft = center;// -m_background.getSize() / 2.f;
	m_background.setPosition(m_topLeft);
	m_foreground.setPosition(m_topLeft);
}

void Healthbar::setColors(const sf::Color& background, const sf::Color& foreground)
{
	m_background.setFillColor(background);
	m_foreground.setFillColor(foreground);
}

void Healthbar::setValue(float value)
{
	if (value < 0)
		value = 0;
	else if (value > 1.f)
		value = 1.f;

	m_foreground.setSize(sf::Vector2f(m_background.getSize().x * value, m_foreground.getSize().y));
}

void Healthbar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_foreground);
}
