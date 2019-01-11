#include "RunButton.h"
#include "C_Client.h"

RunButton::RunButton()
	: m_active(false)
{
	m_tex = ResourceLoader::get().getTexture("assets/graphics/gui/runningmode.png");
	m_shape.setTexture(m_tex);
	m_shape.setSize(sf::Vector2f((float)m_tex->getSize().x / 2.f, (float)m_tex->getSize().y));
	m_shape.setPosition(sf::Vector2f(523, 4));
	setActive(m_active); // Sets the proper texture
}

RunButton::~RunButton()
{

}

void RunButton::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{
	if (ev.type == sf::Event::MouseButtonPressed)
		if (ev.mouseButton.button == sf::Mouse::Button::Left)
			if (m_shape.getGlobalBounds().contains(mousePos))
			{
				// Toggle the run state
				setActive(!m_active);
				auto hidden = CP_SetRun(m_active);
				C_Client::get().getPacket().write(hidden);
			}
}

void RunButton::update(const GameTime& time, const sf::Vector2f& mousePos)
{
	
}


// Called from being added to C_WorldScene::m_uiComponents
void RunButton::draw(sf::RenderTarget& target) const
{
	target.draw(m_shape);
}

void RunButton::setActive(const bool& active)
{
	m_active = active;

	// Set the texture rectangle
	const auto& shapeSize = m_shape.getSize();
	auto rect = sf::IntRect(m_active ? (int)shapeSize.x : 0, 0, (int)shapeSize.x, (int)shapeSize.y);
	m_shape.setTextureRect(rect);
}
