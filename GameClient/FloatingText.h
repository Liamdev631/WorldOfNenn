#pragma once
#include "UIComponent.h"

class FloatingText : public UIComponent
{
private:
	sf::Text m_text;
	float m_timer;

public:
	FloatingText();
	~FloatingText();

	void setCenter(const sf::Vector2f& position);
	void setText(const std::wstring& text);
	bool isExpired() const;

	// UIComponent override
	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const;
};

