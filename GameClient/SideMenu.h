#pragma once
#include "UIComponent.h"
#include "MenuTab.h"

class SideMenu : public UIComponent
{
public:
	static SideMenu& get()
	{
		static SideMenu s;
		return s;
	}
	~SideMenu();

	enum Tab : uint8_t
	{
		Inventory = 0,
		ExperienceTable = 1,
		Settings = 2,
		Combat = 3,

		COUNT
	};

	void setTab(Tab tab);

	// UIComponent override
	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const override;

private:
	SideMenu();

	Tab m_activeMenuTab;
	MenuTab* m_menuTabs[COUNT];
};

