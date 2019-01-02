#pragma once
#include <string>
#include <sstream>
#include "UIComponent.h"

constexpr int TextBufferLength = 20;

class Chatbox : public UIComponent
{
private:
	bool m_active; // If active, keyboard input is directed to the chatbox
	sf::Text m_text[TextBufferLength];
	sf::Text m_inputBufferText;
	sf::String m_inputBuffer;

	// const
	const sf::Vector2f ChatboxSize = sf::Vector2f(505, 128);
	const sf::Vector2f ChatboxPos = sf::Vector2f(7, 345);
	const sf::FloatRect m_bounds = sf::FloatRect(ChatboxPos.x, ChatboxPos.y, ChatboxSize.x, ChatboxSize.y);

public:
	Chatbox();
	~Chatbox();

	bool isActive() const
	{ return m_active; }

	void addText(const std::wstring& str);
	void setScrollValue(const float& value);

	// UIComponent override
	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) override;
};

