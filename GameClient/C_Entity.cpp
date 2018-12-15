#include "C_Entity.h"
#include "C_Client.h"
#include "SFML/Main.hpp"
#include "SFML/Graphics.hpp"
#include <string>
#include "C_WorldScene.h"

C_Entity::C_Entity(const u16 uid)
	: uid(uid), entityType(ET_COUNT), m_moveTimer(0), m_translating(false), expired(false), rotation(0)
{
	position = vec2s(0, 0);
	drawPos = sf::Vector2f(position.x, position.y);
	deathAnimation = 1.0f;

	m_sprite_body = ResourceLoader::get().getSprite("assets/graphics/entities/entity_player.png");
	m_sprite_body->sprite.setOrigin(10, 10);
	m_sprite_body->sprite.setScale(16.f / 20.f, 16.f / 20.f);

	m_sprite_bodyPointer = ResourceLoader::get().getSprite("assets/graphics/entities/entity_pointer.png");
	m_sprite_bodyPointer->sprite.setOrigin(0, 1);
	m_sprite_body->sprite.setScale(16.f / 20.f, 16.f / 20.f);

	m_sprite_hitmarker = ResourceLoader::get().getSprite("assets/graphics/entities/hitmarker.png");
	m_sprite_hitmarker->sprite.setOrigin(8, 8);

	m_font_hitsplat = ResourceLoader::get().getFont("assets/fonts/Candara.ttf");
	C_WorldScene::get().components.push_back(this);

	m_healthbar.setSize(14, 4);
}

C_Entity::~C_Entity()
{
	auto c = C_WorldScene::get().components;
	c.erase(std::remove(c.begin(), c.end(), this), c.end());
}

void C_Entity::update(const GameTime& gameTime)
{
	// Decrement hit marker timers
	for (size_t i = 0; i < hitMarkers.size(); i++)
		if (gameTime.deltaTime > hitMarkers[i].timer)
			hitMarkers[i].timer = 0;
		else
			hitMarkers[i].timer -= gameTime.deltaTime;
	while (hitMarkers.size() > 0 && hitMarkers.front().timer == 0)
		hitMarkers.pop_front();

	if (combatState.dead)
	{
		if (m_moveStateHistory.size() > 0)
			m_moveStateHistory = { };

		// Decrement death timer and animation
		if (gameTime.deltaTime <= deathAnimation)
			expired = true;
		else
			deathAnimation -= gameTime.deltaTime;
		return;
	}

	// Movement
	if (m_moveStateHistory.size() > 0) // if moving
	{
		const sf::Vector2f targetPos = sf::Vector2f(
			m_moveStateHistory.front().moveState.pos.x,
			m_moveStateHistory.front().moveState.pos.y);
		const float distanceToTarget = std::sqrtf(std::powf(targetPos.x - drawPos.x, 2) + std::powf(targetPos.y - drawPos.y, 2));
		if (distanceToTarget < 0.1f || distanceToTarget > 3.f)
		{
			// Jump to the position immediatly if we are clsoe enough or too far away
			position = m_moveStateHistory.front().moveState.pos;
			drawPos = targetPos;
			m_moveStateHistory.pop();
		}
		else
		{
			auto velocity = (targetPos - drawPos) / distanceToTarget; // Normalize
			auto cellDiff = m_moveStateHistory.front().moveState.pos - position;
			float stepSize = gameTime.deltaTime * 2.f;
			if (cellDiff.x != 0 && cellDiff.y != 0)
				stepSize *= 1.41f;
			stepSize *= m_moveStateHistory.size(); // TODO: Add smoothing
			velocity = velocity * stepSize;
			drawPos = drawPos + velocity;
		}
	}

	// Update healthbar
	if (combatState.currentHealth < combatState.maxHealth)
	{
		m_healthbar.setValue((float)combatState.currentHealth / (float)combatState.maxHealth);
		const sf::Vector2f healthbarPosition = sf::Vector2f(drawPos.x * 16, drawPos.y * 16 - 10);
		m_healthbar.setCenter(healthbarPosition);
		// Change color if poisoned
		if (combatState.poisoned)
			m_healthbar.setColors(sf::Color::Red, sf::Color::Yellow);
		else
			m_healthbar.setColors(sf::Color::Red, sf::Color::Green);
	}
}

void C_Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (expired)
		return;

	// Body
	m_sprite_body->sprite.setPosition(drawPos.x * 16.f + 8.0f, drawPos.y * 16.f + 8.0f);
	m_sprite_body->sprite.setColor(getColor());
	target.draw(*m_sprite_body);

	// Pointer
	m_sprite_bodyPointer->sprite.setPosition(drawPos.x * 16.f + 8.0f, drawPos.y * 16.f + 8.0f);
	m_sprite_bodyPointer->sprite.setRotation(rotation * 45.f);
	target.draw(*m_sprite_bodyPointer);

	// Healthbar
	if (combatState.currentHealth < combatState.maxHealth)
	{
		target.draw(m_healthbar);
	}

	// Hitmarkers
	/*int count = 0;
	for (auto hit = hitMarkers.begin(); hit != hitMarkers.end(); hit++)
	{
		sf::CircleShape circle = sf::CircleShape(8.0f, 12);
		circle.setPosition((float)drawPos.x * 16.f, (drawPos.y * 16.f - 16.f) + (hit->timer * 10) - (count * 16));
		if (hit->damage == 0)
			circle.setFillColor(sf::Color::Blue);
		else
			circle.setFillColor(sf::Color::Red);

		sf::Text text;
		text.setPosition(circle.getPosition());
		text.setFont(*m_font_hitsplat);
		text.setCharacterSize(12);
		text.setFillColor(sf::Color::Black);

		target.draw(circle);
		target.draw(text);
		count++;
	}*/

}

const sf::Color C_Entity::getColor() const
{
	switch (entityType)
	{
		case ET_ADMIN:
			return sf::Color::Cyan;
		case ET_PLAYER:
			return sf::Color::White;
		default:
			return sf::Color::Yellow;
	}
}

void C_Entity::addMoveStateKey(const MoveStateKey& key)
{
	m_moveStateHistory.push(key);

	//position = key.moveState.pos;
	//rotation = key.moveState.rot;
	//interpPosition = vec2f((float)position.x, (float)position.y) * 16;
}
