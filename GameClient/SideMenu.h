#pragma once
#include "UIComponent.h"
#include "MenuTab.h"

enum SideMenuTab : uint8_t
{
	Tab_Inventory = 0,
	Tab_Experience = 1,
	Tab_Settings = 2,
	Tab_Combat = 3,
	Tab_COUNT
};

constexpr int NumTabs = 16;
constexpr SideMenuTab DefaultTab = SideMenuTab::Tab_Inventory;

class SideMenu : public UIComponent
{
public:
	static SideMenu& get()
	{
		static SideMenu s;
		return s;
	}
	~SideMenu();

	void setTab(SideMenuTab tab);

	// UIComponent override
	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const override;

private:
	SideMenu();

	SideMenuTab m_activeMenuTab;
	MenuTab* m_menuTabs[NumTabs];
	sf::RectangleShape m_tabButtons[NumTabs];
};

