#include "RunButton.h"
#include "C_Client.h"

RunButton::RunButton()
	: m_active(false)
{
	m_sprite = &ResourceLoader::get().getSprite("assets/graphics/gui/runningmode.png")->sprite;
	m_sprite->setPosition(sf::Vector2f(523, 4));
	setActive(m_active); // Sets the proper texture
}

RunButton::~RunButton()
{

}

void RunButton::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{
	if (ev.type == sf::Event::MouseButtonPressed)
		if (ev.mouseButton.button == sf::Mouse::Button::Left)
			if (m_sprite->getGlobalBounds().contains(mousePos))
			{
				setActive(!m_active); // Toggle
				auto p = CP_SetRun(m_active);
				C_Client::get().getPacket().write(p);
			}
}

void RunButton::update(const sf::Vector2f& mousePos)
{

}


// Called from being added to C_WorldScene::m_uiComponents
void RunButton::draw(sf::RenderTarget& target)
{
	target.draw(*m_sprite);
}

void RunButton::setActive(const bool& active)
{
	m_active = active;

	// Set the texture rectangle
	const auto& texSize = m_sprite->getTexture()->getSize();
	int iconWidth = texSize.x / 2;
	auto rect = sf::IntRect(m_active ? iconWidth : 0, 0, iconWidth, texSize.y);
	m_sprite->setTextureRect(rect);
}
