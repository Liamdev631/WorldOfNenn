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

void ExperienceTable::addExp(Skill s, exp_val exp)
{
	m_experience[s] += exp;
	calcLevel(s);
}

void ExperienceTable::addExp(const std::vector<exp_chunk>& exp)
{
	for (auto pair : exp)
	{
		m_experience[pair.first] += pair.second;
		calcLevel(pair.first);
	}
}

void ExperienceTable::setExp(Skill s, exp_val exp)
{
	m_experience[s] = exp;
	calcLevel(s);
}

void ExperienceTable::calcLevel(Skill s)
{
	m_levels[s] = 1 + (level_val)sqrtf(((float)m_experience[s] + 1.f) / 100.f);
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

