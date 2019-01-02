#pragma once
#include "Global.h"

#define SK_Count 10

typedef u8 level_val;
typedef u32 exp_val;


enum Skill : u8
{
	SK_Strength = 0,
	SK_Constitution = 1,
	SK_Defence = 2,
	SK_Dexterity = 3,
	SK_Intelligence = 4,
	SK_Charisma = 5,
	SK_Wisdom = 6,
	SK_Willpower = 7,
	SK_Perception = 8,
	SK_Luck = 9,
};

class ExperienceTable
{
private:
	level_val m_levels[SK_Count];
	exp_val m_experience[SK_Count];

public:
	ExperienceTable();
	~ExperienceTable();

	void setExp(Skill s, exp_val exp);
	void calcLevel(Skill s);
	void calcAllLevels();
	level_val getLevel(Skill s) const;
	exp_val getExperience(Skill s) const;
};

