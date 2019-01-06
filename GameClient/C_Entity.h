#pragma once
#include <queue>
#include "Global.h"
#include "Packets.h"
#include "SFML/Graphics.hpp"
#include "GameTime.h"
#include "ResourceLoader.h"
#include "Healthbar.h"
#include "FloatingText.h"

struct HitMarker
{
	float timer;
	const u8 damage;

	HitMarker(const u8 damage)
		: damage(damage), timer(0.8f)
	{

	}
};

class C_Entity : public sf::Drawable
{
public:
	const u16 uid;
	vec2s position;
	u8 rotation = 0;
	EntityType entityType;
	bool expired;
	float deathAnimationTimer;
	CombatState combatState;
	std::deque<HitMarker> hitMarkers;

	// Animation
	sf::Vector2f drawPos;
	float drawRot;

private:
	std::queue<MoveKey> moveStateHistory;

	float m_moveTimer;
	bool m_translating;

	Sprite* m_sprite_body;
	Sprite* m_sprite_bodyPointer;
	Sprite* m_sprite_hitmarker;
	sf::Font* m_font_hitsplat;
	Healthbar m_healthbar;
	FloatingText m_floatingText;

	float m_counter = 0;

public:
	C_Entity(const u16 uid);
	void loadAssets();
	~C_Entity();

	void update(const GameTime& gameTime); // 60 times/sec
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	const sf::Color getColor() const;

	void addMoveKey(const MoveKey& key);
	void setFloatingText(const std::wstring& text);

private:
	
};
