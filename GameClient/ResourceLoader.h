#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Items.h"

class Sprite : public sf::Drawable
{
public:
	sf::Texture texture;
	sf::Sprite sprite;

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override
	{
		rt.draw(sprite);
	}
};

class ResourceLoader
{
private:
	ResourceLoader();

	std::map<std::string, Sprite> spriteMap;
	std::map<std::string, sf::Font> fontMap;

public:
	~ResourceLoader();

	// Singleton accessor
	static ResourceLoader& get()
	{
		static ResourceLoader r;
		return r;
	}

	Sprite* getSprite(const std::string& fileName);
	sf::Sprite& getItemSprite(const ItemStack& item);
	sf::Font* getFont(const std::string& fileName);
};

