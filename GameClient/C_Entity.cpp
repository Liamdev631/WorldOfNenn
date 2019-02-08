#include "C_Entity.h"
#include "C_Client.h"
#include "SFML/Main.hpp"
#include "SFML/Graphics.hpp"
#include <string>
#include "SceneManager.h"
#include <math.h>
#include <string>

C_Entity::C_Entity(const u16 uid)
	: uid(uid), entityType(ET_COUNT), m_moveTimer(0), m_translating(false), expired(false), rotation(0)
{
	C_WorldManager::get().getActiveEntities().push_back(this);

	position = vec2s(0, 0);
	drawPos = sf::Vector2f(0, 0);
	deathAnimationTimer = 2.0f;
	m_healthbar.setSize(14, 4);

	loadAssets();

}

void C_Entity::loadAssets()
{
	//m_sprite_hitmarker = ResourceLoader::get().getTexture("assets/graphics/entities/hitmarker.png");
	//m_sprite_hitmarker->sprite.setOrigin(8, 8);

	m_font_hitsplat = ResourceLoader::get().getFont("assets/fonts/Candara.ttf");

	m_bodyShape.setSize(sf::Vector2f(16.f, 16.f));
}

C_Entity::~C_Entity()
{
	auto c = C_WorldManager::get().getActiveEntities();
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
			
			// Move faster along diagonals (FIX)
			//auto cellDiff = moveKey.pos - position;
			//if (cellDiff.x != 0 && cellDiff.y != 0)
			//	positionStepSize *= 1.41f;

			velocity = velocity * positionStepSize * 0.95f;
			drawPos = drawPos + velocity;

			// Integrate for rotation
			//float rotationStepSize = gameTime.deltaTime;
			drawRot = (360 * moveKey.rot) / 8.f;
		}
	}
	auto snappedDrawPos = drawPos * 16.f;
	snappedDrawPos.x = (float)((int)snappedDrawPos.x);
	snappedDrawPos.y = (float)((int)snappedDrawPos.y);
	m_bodyShape.setPosition(snappedDrawPos);

	// Update healthbar
	if (combatState.currentHealth < combatState.maxHealth)
	{
		m_healthbar.setValue((float)combatState.currentHealth / (float)combatState.maxHealth);
		sf::Vector2f healthbarPosition = sf::Vector2f(snappedDrawPos.x, snappedDrawPos.y - 10);
		healthbarPosition.x = std::floorf(healthbarPosition.x);
		healthbarPosition.y = std::floorf(healthbarPosition.y);
		m_healthbar.setCenter(healthbarPosition);
		// Change color if poisoned
		if (combatState.poisoned)
			m_healthbar.setColors(sf::Color::Red, sf::Color::Yellow);
		else
			m_healthbar.setColors(sf::Color::Red, sf::Color::Green);
	}

	// Update the floating text
	if (!m_floatingText.isExpired())
	{
		m_floatingText.update(gameTime, sf::Vector2f(0, 0));
		m_floatingText.setCenter(snappedDrawPos + sf::Vector2f(0, -24));
	}
}

void C_Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (expired)
		return;

	target.draw(m_bodyShape);

	// Healthbar
	if (combatState.currentHealth < combatState.maxHealth)
	{
		target.draw(m_healthbar);
	}

	// Hitmarkers
	int count = 0;
	for (auto hit = hitMarkers.begin(); hit != hitMarkers.end(); hit++)
	{
		sf::CircleShape circle = sf::CircleShape(6.0f, 8);
		auto hitmarkerPos = sf::Vector2f((float)drawPos.x * 16.f, (drawPos.y * 16.f - 32.f) + (hit->timer * 6.f) - (count * 14));
		hitmarkerPos.x = (float)((int)hitmarkerPos.x);
		hitmarkerPos.y = (float)((int)hitmarkerPos.y); // Lock to integer coordinates
		circle.setPosition(hitmarkerPos);
		if (hit->damage == 0)
			circle.setFillColor(sf::Color::Blue);
		else
			circle.setFillColor(sf::Color::Red);

		sf::Text text;
		text.setPosition(circle.getPosition() + sf::Vector2f(4, -3));
		text.setFont(*m_font_hitsplat);
		text.setCharacterSize(12);
		text.setFillColor(sf::Color::Black);
		text.setString(std::to_string(hit->damage));

		target.draw(circle);
		target.draw(text);
		count++;
	}

	// Floating chat text
	if (!m_floatingText.isExpired())
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

const EntityType C_Entity::getEntityType() const
{
	return entityType;
}

void C_Entity::setEntityType(EntityType type)
{
	entityType = type;

	// Set the entities sprite based on EntityType
	m_bodyShape.setTexture(ResourceLoader::get().getEntityTexture(entityType));
}

const sf::FloatRect C_Entity::getGlobalBounds() const
{
	auto bounds = m_bodyShape.getGlobalBounds();
	return bounds;
}
