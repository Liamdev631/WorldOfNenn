#include "C_SpriteCache.h"

C_SpriteCache::C_SpriteCache()
{
	
}

C_SpriteCache::~C_SpriteCache()
{

}

void C_SpriteCache::loadContent()
{
	for (size_t i = 0; i < NUM_ITEMS; i++)
	{
		const std::string fileName = "graphics/items/item_" + std::to_string(i) + ".png";
		if (!m_itemTextures[i].loadFromFile(fileName)) {
			printf("Failed to load sprite %s!\n", fileName.c_str());
			continue;
		}
		m_itemSprites[i].setTexture(m_itemTextures[i]);
	}
}

void C_SpriteCache::drawItemSprite(sf::RenderWindow& window, const ItemType& item, const sf::Vector2f& position, const float& scale)
{
	sf::Sprite& sprite = m_itemSprites[item];
	sprite.setPosition(sf::Vector2f(position.x, position.y));
	sprite.setScale(scale, scale);
	window.draw(sprite);
}

void C_SpriteCache::drawItemSprite(sf::RenderWindow& window, const ItemType& item, const Vec2s& position, const float& scale)
{
	drawItemSprite(window, item, sf::Vector2f(position.x, position.y), scale);
}
