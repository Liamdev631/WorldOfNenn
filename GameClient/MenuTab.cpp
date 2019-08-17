#include "MenuTab.h"

MenuTab::MenuTab()
	: m_menuBounds(sf::FloatRect(527, 205, 230, 261))
{

}

MenuTab::~MenuTab()
{

}

const sf::FloatRect& MenuTab::getGlobalBounds()
{
	return m_menuBounds;
}
