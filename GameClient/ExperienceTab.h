#pragma once
#include "MenuTab.h"
#include "Items.h"
#include "Skills.h"

#ifndef TabBounds
#define TabBounds sf::FloatRect(547, 205, 190, 261)
#endif

class ExperienceTab : public MenuTab
{
private:
	ExperienceTab();

	struct ExpColumn : public UIComponent
	{
	private:
		sf::RectangleShape icon;
		exp_val currExp;
		sf::Text text;
		Skill m_skill;

	public:
		ExpColumn() { };
		ExpColumn(Skill s)
			: m_skill(s)
		{
			auto pos = sf::Vector2f(TabBounds.left + 6.f, TabBounds.top + 6.f + (float)s * 16.0f);

			// Skill icon
			//auto texture = &ResourceLoader::get().getSprite("assets/graphics/gui/menu_" + std::to_string((int)s))->texture;
			//icon.setTexture(texture);
			icon.setSize(sf::Vector2f(14, 14));
			icon.setPosition(pos);

			// SKill text
			auto& font = *ResourceLoader::get().getFont("assets/fonts/Candara.ttf");
			text.setFont(font);
			text.setCharacterSize(14);
			text.setPosition(pos + sf::Vector2f(16, -3));

			// Set to default values
			setExp(0);
		}

		void setExp(exp_val exp)
		{
			currExp = exp;
			text.setString(getSkillName(m_skill) + L" exp: " + std::to_wstring(exp));
		}

		void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override
		{

		}

		void update(const GameTime& time, const sf::Vector2f& mousePos) override
		{

		}

		void draw(sf::RenderTarget& target) const override
		{
			target.draw(icon);
			target.draw(text);
		}

	};
	
	ExperienceTable m_currentTable;
	ExpColumn m_columns[SK_Count];

public:
	~ExperienceTab();
	static ExperienceTab& get()
	{
		static ExperienceTab e;
		return e;
	}

	void setExperienceTable(const ExperienceTable& table);

	// UIComponent override
	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const override;
};