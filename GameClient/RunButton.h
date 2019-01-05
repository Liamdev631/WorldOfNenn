#pragma once
#include "UIComponent.h"

class RunButton : public UIComponent
{
private:
	bool m_active;
	sf::Sprite* m_sprite;

public:
	static RunButton& get()
	{
		static RunButton r;
		return r;
	}
	~RunButton();

	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const override;

	void setActive(const bool& active);

private:
	RunButton();
};

