#include "C_Entity.h"
#include "C_Client.h"
#include "SFML/Main.hpp"
#include "SFML/Graphics.hpp"
#include <string>
#include "C_WorldScene.h"
#include <math.h>
#include <string>

C_Entity::C_Entity(const u16 uid)
	: uid(uid), entityType(ET_COUNT), m_moveTimer(0), m_translating(false), expired(false), rotation(0)
{
	position = vec2s(0, 0);
	drawPos = sf::Vector2f(position.x, position.y);
	deathAnimationTimer = 2.0f;
	m_healthbar.setSize(14, 4);

	loadAssets();

	SceneManager::get().components.push_back(this);
}

void C_Entity::loadAssets()
{
	m_sprite_body = ResourceLoader::get().getSprite("assets/graphics/entities/entity_player.png");
	m_sprite_body->sprite.setOrigin(10, 10);
	m_sprite_body->sprite.setScale(16.f / 20.f, 16.f / 20.f);
	m_sprite_bodyPointer = ResourceLoader::get().getSprite("assets/graphics/entities/entity_pointer.png");
	m_sprite_bodyPointer->sprite.setOrigin(0, 1);
	m_sprite_body->sprite.setScale(16.f / 20.f, 16.f / 20.f);
	m_sprite_hitmarker = ResourceLoader::get().getSprite("assets/graphics/entities/hitmarker.png");
	m_sprite_hitmarker->sprite.setOrigin(8, 8);
	m_font_hitsplat = ResourceLoader::get().getFont("assets/fonts/Candara.ttf");
}

C_Entity::~C_Entity()
{
	auto c = SceneManager::get().components;
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
		if (moveStateHistory.size() > 0)
			moveStateHistory = { };

		// Decrement death timer and animation
		if (gameTime.deltaTime <= deathAnimationTimer)
		{
			expired = true;
			deathAnimationTimer = 2.0f;
		}
		else
			deathAnimationTimer -= gameTime.deltaTime;
		return;
	}

	// Movement
	if (moveStateHistory.size() > 0) // if moving
	{
		auto& moveKey = moveStateHistory.front();
		printf("%u\n", moveKey.speed);
		const sf::Vector2f targetPos = sf::Vector2f(moveKey.pos.x, moveKey.pos.y);
		const float distanceToTarget = std::sqrtf(std::powf(targetPos.x - drawPos.x, 2) + std::powf(targetPos.y - drawPos.y, 2));
		if (distanceToTarget < 0.1f || distanceToTarget > 3.f)
		{
			// Jump to the position immediatly if we are close enough or too far away
			position = moveKey.pos;
			rotation = moveKey.rot;
			drawPos = targetPos;
			moveStateHistory.pop();
		}
		else
		{
			// Integrate for position
			auto velocity = (targetPos - drawPos) / distanceToTarget; // Normalize
			float positionStepSize = gameTime.deltaTime * S_TICKS_PER_SECOND / moveKey.speed;
			// Move faster along diagonals
			auto cellDiff = moveKey.pos - position;
			if (cellDiff.x != 0 && cellDiff.y != 0)
				positionStepSize *= 1.41f;

			velocity = velocity * positionStepSize;
			drawPos = drawPos + velocity;

			// Integrate for rotation
			//float rotationStepSize = gameTime.deltaTime;
			drawRot = (360 * moveKey.rot) / 8.f;
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

	// Update the floating text
	m_floatingText.update(gameTime, sf::Vector2f(0, 0));
	m_floatingText.setCenter(drawPos * 16.f + sf::Vector2f(240, 150));
	//m_floatingText.setText(L"NIGGERZ!");
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
	m_sprite_bodyPointer->sprite.setRotation(drawRot);
	target.draw(*m_sprite_bodyPointer);

	// Healthbar
	if (combatState.currentHealth < combatState.maxHealth)
	{
		target.draw(m_healthbar);
	}

	// Hitmarkers
	int count = 0;
	for (auto hit = hitMarkers.begin(); hit != hitMarkers.end(); hit++)
	{
		sf::CircleShape circle = sf::CircleShape(8.0f, 8);
		circle.setPosition((float)drawPos.x * 16.f, (drawPos.y * 16.f - 16.f) + (hit->timer * 10) - (count * 16));
		if (hit->damage == 0)
			circle.setFillColor(sf::Color::Blue);
		else
			circle.setFillColor(sf::Color::Red);

		sf::Text text;
		text.setPosition(circle.getPosition() + sf::Vector2f(4, 0));
		text.setFont(*m_font_hitsplat);
		text.setCharacterSize(12);
		text.setFillColor(sf::Color::Black);
		text.setString(std::to_string(hit->damage));

		target.draw(circle);
		target.draw(text);
		count++;
	}

	// Floating chat text
	m_floatingText.draw(target);
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

void C_Entity::addMoveKey(const MoveKey& key)
{
	moveStateHistory.push(key);
}

void C_Entity::setFloatingText(const std::wstring& text)
{
	m_floatingText.setText(text);
}
