#pragma once
#include <string>
#include <SFML/Graphics.hpp>

constexpr int TextBufferLength = 20;

class Chatbox : public sf::Drawable
{
private:
	sf::Text m_text[TextBufferLength];

	// const
	const sf::Vector2u ChatboxSize = sf::Vector2u(505, 128);
	const sf::Vector2u ChatboxPos = sf::Vector2u(7, 345);

public:
	Chatbox();
	~Chatbox();

	void addText(const std::wstring& str);

	void setScrollValue(const float& value);

	// sf::Drawable override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

