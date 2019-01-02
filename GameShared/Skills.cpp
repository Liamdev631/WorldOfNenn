#include "Skills.h"

ExperienceTable::ExperienceTable()
{
	for (auto i = 0; i < SK_Count; i++)
	{
		m_experience[i] = 0;
		m_levels[i] = 1;
	}
}

ExperienceTable::~ExperienceTable()
{

}

void ExperienceTable::setExp(Skill s, exp_val exp)
{
	m_experience[s] = exp;
	calcLevel(s);
}

void ExperienceTable::calcLevel(Skill s)
{
	m_levels[s] = 1 + (level_val)sqrtf(m_experience[s] + 1);
}

void ExperienceTable::calcAllLevels()
{
	for (auto i = 0; i < SK_Count; i++)
		calcLevel((Skill)i);
}

exp_val ExperienceTable::getExperience(Skill s) const
{
	return m_experience[s];
}

level_val ExperienceTable::getLevel(Skill s) const
{
	//calcLevel(s)
	return m_levels[s];
}

