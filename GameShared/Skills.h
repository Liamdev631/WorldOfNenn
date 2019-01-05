#pragma once
#include "Global.h"
#include <utility>
#include <vector>

#define SK_Count 9

typedef u8 level_val;
typedef u32 exp_val;

enum Skill : u8
{
	SK_Strength = 0,
	SK_Constitution = 1,
	SK_Defence = 2,
	SK_Dexterity = 3,
	SK_Perception = 4,
	SK_Intelligence = 5,
	SK_Wisdom = 6,
	SK_Luck = 7,
	SK_Prayer = 8,
};

typedef std::pair<Skill, exp_val> exp_chunk;

class ExperienceTable
{
private:
	level_val m_levels[SK_Count];
	exp_val m_experience[SK_Count];

public:
	ExperienceTable();
	~ExperienceTable();

	void addExp(Skill s, exp_val exp);
	void addExp(const std::vector<exp_chunk>& exp);
	void setExp(Skill s, exp_val exp);
	void calcLevel(Skill s);
	void calcAllLevels();
	level_val getLevel(Skill s) const;
	exp_val getExperience(Skill s) const;
};
