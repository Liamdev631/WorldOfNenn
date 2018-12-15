#pragma once
#include "Global.h"
#include "CommonStructs.h"

#define NUM_ITEMS 1

class C_SpriteCache
{
private:
	//sf::Texture m_itemTextures[NUM_ITEMS];
	//sf::Sprite m_itemSprites[NUM_ITEMS];

public:
	C_SpriteCache();
	~C_SpriteCache();

	void loadContent();

	void drawItemSprite(sf::RenderWindow& window, const ItemType& item, const Vec2f& position, const float& scale = 1.f);
	void drawItemSprite(sf::RenderWindow& window, const ItemType& item, const Vec2s& position, const float& scale = 1.f);
};

