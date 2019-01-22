#include "FloatingText.h"
#include "ResourceLoader.h"

constexpr float TextDisplayDuration = 200.5f; // Seconds

FloatingText::FloatingText()
{
	auto font = ResourceLoader::get().getFont("assets/fonts/Candara.ttf");
	m_text = sf::Text("", *font, 15);
	m_text.setFillColor(sf::Color::Black);
}

FloatingText::~FloatingText()
{

}

void FloatingText::setCenter(const sf::Vector2f& position)
{
	//auto centerCharPos = m_text.findCharacterPos(min(m_text.getString().getSize() / 2 + 1, m_text.getString().getSize()));
	auto centerCharPos = m_text.findCharacterPos(m_text.getString().getSize()) / 2.f;
	m_text.setPosition(position - centerCharPos);
}

void FloatingText::setText(const std::wstring& text)
{
	m_text.setString(text);
	m_timer = TextDisplayDuration;
}

bool FloatingText::isExpired() const
{
	return (m_timer <= 0);
}

// UIComponent override

void FloatingText::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{

}

void FloatingText::update(const GameTime& time, const sf::Vector2f& mousePos)
{
	m_timer -= time.deltaTime;
	//printf("%f\n", m_timer);
}

void FloatingText::draw(sf::RenderTarget& target) const
{
	if (isExpired())
		return;
	target.draw(m_text);
}

