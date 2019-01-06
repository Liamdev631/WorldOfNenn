#include "SideMenu.h"
#include "InventoryTab.h"
#include "ExperienceTab.h"

SideMenu::SideMenu()
{
	// Setup menu tabs
	for (int i = 0; i < NumTabs; i++)
		m_menuTabs[i] = nullptr; // Tabs are null by default
	m_menuTabs[SideMenuTab::Tab_Inventory] = new InventoryTab();
	m_menuTabs[SideMenuTab::Tab_Experience] = new ExperienceTab();

	// Create the buttons for switching tabs
	sf::Vector2f tabPos = sf::Vector2f(522, 168);
	sf::Vector2f tabSize = sf::Vector2f(30, 37);

	for (int y = 0; y < 2; y++)
		for (int x = 0; x < 8; x++)
		{
			auto& tab = m_tabButtons[x + y * 8];
			tab = sf::RectangleShape(tabSize);
			tab.setPosition(tabPos + sf::Vector2f(tabSize.x * x, 298.f * y));
			tab.setSize(tabSize);
			tab.setOutlineColor(sf::Color::Red);
			tab.setFillColor(sf::Color::Transparent);
		}
	setTab(DefaultTab);
}

SideMenu::~SideMenu()
{
	for (int i = 0; i < SideMenuTab::Tab_COUNT; i++)
		if (m_menuTabs[i])
			delete m_menuTabs[i];
}

void SideMenu::setTab(SideMenuTab tab)
{
	//if (m_menuTabs[tab] == nullptr)
	//	return; // Tab hasn't been designed yet
	m_activeMenuTab = tab;

	// Draw an outline around the button for the selected tab
	for (int i = 0; i < 16; i++)
		m_tabButtons[i].setOutlineThickness(0);
	m_tabButtons[tab].setOutlineThickness(1);
}

void SideMenu::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{
	// Switch tabs if a tab button has been pressed
	if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Button::Left)
	{
		for (int i = 0; i < 16; i++)
			if (m_tabButtons[i].getGlobalBounds().contains(mousePos))
			{
				setTab((SideMenuTab)i);
				return;
			}
	}

	// Pass the event to the active window
	auto& activeTab = m_menuTabs[m_activeMenuTab];
	if (activeTab)
		activeTab->onEvent(ev, mousePos);
}

void SideMenu::update(const GameTime& time, const sf::Vector2f& mousePos)
{
	auto& activeTab = m_menuTabs[m_activeMenuTab];
	if (activeTab)
		activeTab->update(time, mousePos);
}

void SideMenu::draw(sf::RenderTarget& target) const
{
	// Draw the tab switching buttons
	for (int i = 0; i < 16; i++)
		target.draw(m_tabButtons[i]);

	// Only draw the active tab
	auto& activeTab = m_menuTabs[m_activeMenuTab];
	if (activeTab)
		activeTab->draw(target);
}
