#pragma once
#include "MenuTab.h"
#include "Items.h"

class ExperienceTab : public MenuTab
{
protected:


private:

public:
	ExperienceTab();
	~ExperienceTab();

	// UIComponent override
	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const override;
};

