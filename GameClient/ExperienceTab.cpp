#include "ExperienceTab.h"
#include "C_Client.h"
#include "ResourceLoader.h"
#include "Packets.h"
#include "C_WorldScene.h"

ExperienceTab::ExperienceTab()
{
	for (int i = 0; i < SK_Count; i++)
		m_columns[i] = ExpColumn((Skill)i);
}

ExperienceTab::~ExperienceTab()
{

}

void ExperienceTab::setExperienceTable(const ExperienceTable& table)
{
	for (int i = 0; i < SK_Count; i++)
	{
		Skill s = (Skill)i;
		if (m_currentTable.getExp(s) != table.getExp(s))
		{
			m_currentTable.setExp(s, table.getExp(s));
			m_columns[i].setExp(table.getExp(s));
		}
	}
}

// UIComponent override

void ExperienceTab::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{
	for (int i = 0; i < SK_Count; i++)
		m_columns[i].onEvent(ev, mousePos);
}

void ExperienceTab::update(const GameTime& time, const sf::Vector2f& mousePos)
{
	for (int i = 0; i < SK_Count; i++)
		m_columns[i].update(time, mousePos);
}

void ExperienceTab::draw(sf::RenderTarget& target) const
{
	for (int i = 0; i < SK_Count; i++)
		m_columns[i].draw(target);
}
