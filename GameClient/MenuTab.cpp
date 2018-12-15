#include "MenuTab.h"

MenuTab::MenuTab()
	: m_menuBounds(sf::FloatRect(547, 205, 190, 261))
{

}

MenuTab::~MenuTab()
{

}

const sf::FloatRect& MenuTab::getBounds()
{
	return m_menuBounds;
}
