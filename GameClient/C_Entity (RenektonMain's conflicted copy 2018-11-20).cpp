#include "C_Entity.h"
#include "C_Client.h"
#include "SFML/Main.hpp"
#include "SFML/Graphics.hpp"
#include <string>

C_Entity::C_Entity(const u16 uid)
	: uid(uid), entityType(ET_COUNT), m_moveTimer(0), m_translating(false), expired(false), rotation(0)
{
	position = vec2s(0, 0);
	interpPosition = Vec2f(position.x, position.y);
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
}

C_Entity::~C_Entity()
{

}

u16 C_Entity::getUID() const
{
	return uid;
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

	// Decrement death timer
	if (combatState.dead)
	{
		if (gameTime.deltaTime >= deathAnimation)
			expired = true;
		else
			deathAnimation -= gameTime.deltaTime;
		return;
	}

	// Movement
	if (m_moveStateHistory.size() > 0) // if moving
	{
		const vec2s& targetPos = m_moveStateHistory.front().moveState.pos;
		Vec2f velocity = Vec2f((float)targetPos.x - interpPosition.x, (float)targetPos.y - interpPosition.y);
		if (velocity.length() <= 0.05f) // Lock to position if close enough
		{
			//printf("x:%u, y:%u\n", m_moveStateHistory.front().moveState.pos.x, m_moveStateHistory.front().moveState.pos.y);
			position = targetPos;
			interpPosition = Vec2f((float)position.x, (float)position.y);
			m_moveStateHistory.pop();
		}
		else if (velocity.length() > 3) // Jump to position if too far away
		{
			position = targetPos;
			interpPosition = Vec2f((float)position.x, (float)position.y);
			m_moveStateHistory.pop();
		}
		else
		{
			velocity.normalize();
			velocity.x *= 0.05f;
			velocity.y *= 0.05f;
			interpPosition = interpPosition + velocity;
			if (uid == 0)
				printf("%u, %u\n", (int)(interpPosition.x / 16), (int)(interpPosition.y / 16));
		}
	}
}

constexpr bool UseSmoothMoving = false;
void C_Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (expired)
		return;

	// Body
	m_sprite_body->sprite.setPosition(interpPosition.x * 16.f + 8.0f, interpPosition.y * 16.f + 8.0f);
	m_sprite_body->sprite.setColor(getColor());

	// Pointer
	m_sprite_bodyPointer->sprite.setPosition(interpPosition.x * 16.f + 8.0f, interpPosition.y * 16.f + 8.0f);
	m_sprite_bodyPointer->sprite.setRotation(rotation * 45.f);

	// Hitmarkers
	int count = 1;
	for (auto hit = hitMarkers.begin(); hit != hitMarkers.end(); hit++)
	{
		sf::CircleShape circle = sf::CircleShape(8.0f, 12);
		circle.setPosition((float)interpPosition.x * 16.f, interpPosition.y * 16.f - 16.f - hit->timer * 10.0f);
		if (hit->damage == 0)
			circle.setFillColor(sf::Color::Blue);
		else
			circle.setFillColor(sf::Color::Red);

		sf::Text text;
		text.setPosition(circle.getPosition() + sf::Vector2f(2, -2));
		text.setFont(*m_font_hitsplat);
		text.setCharacterSize(12);
		text.setFillColor(sf::Color::Black);
		text.setString("Welcome to SFML pong!\nPress space to start the game");

		target.draw(circle);
		target.draw(text);
		count++;
	}

	// Healthbar
	if (combatState.currentHealth < combatState.maxHealth)
	{
		const sf::Vector2f healthbarPosition = sf::Vector2f(interpPosition.x * 16.f, interpPosition.y * 16.f - 10.f);
		
		sf::RectangleShape healthbarBackground = sf::RectangleShape();
		sf::RectangleShape healthbarForeground = sf::RectangleShape();

		healthbarBackground.setSize(sf::Vector2f(14, 3));
		healthbarBackground.setFillColor(sf::Color::Red);
		healthbarBackground.setOutlineThickness(1.0f);
		healthbarBackground.setOutlineColor(sf::Color::Black);
		healthbarBackground.setPosition(healthbarPosition);

		healthbarForeground.setSize(sf::Vector2f(12.f * (float)combatState.currentHealth / (float)combatState.maxHealth, 3.f));
		healthbarForeground.setPosition(healthbarPosition);
		
		target.draw(healthbarBackground);
		target.draw(healthbarForeground);
	}

	target.draw(*m_sprite_body);
	target.draw(*m_sprite_bodyPointer);
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

	//m_moveStateHistory.push(key);
	//position = key.moveState.pos;
	//rotation = key.moveState.rot;
	//interpPosition = Vec2f((float)position.x, (float)position.y) * 16.f;
}
