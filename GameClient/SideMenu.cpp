#include "SideMenu.h"
#include "InventoryTab.h"

SideMenu::SideMenu()
{
	m_activeMenuTab = Tab::Inventory;
	for (int i = 0; i < Tab::COUNT; i++)
		m_menuTabs[i] = nullptr; // Tabs are null by default
	// Create each menu tab object
	m_menuTabs[Tab::Inventory] = new InventoryTab();
}

SideMenu::~SideMenu()
{
	for (int i = 0; i < Tab::COUNT; i++)
		if (m_menuTabs[i])
			delete m_menuTabs[i];
}

void SideMenu::setTab(Tab tab)
{
	if (!m_menuTabs[tab])
		return;
	m_activeMenuTab = tab;
}

void SideMenu::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{
	auto& activeTab = m_menuTabs[m_activeMenuTab];
	activeTab->onEvent(ev, mousePos);
}

void SideMenu::update(const sf::Vector2f& mousePos)
{
	auto& activeTab = m_menuTabs[m_activeMenuTab];
	activeTab->update(mousePos);
}

void SideMenu::draw(sf::RenderTarget& target)
{
	auto& activeTab = m_menuTabs[m_activeMenuTab];
	activeTab->draw(target);
}
