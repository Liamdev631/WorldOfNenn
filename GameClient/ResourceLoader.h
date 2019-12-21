#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Items.h"
#include "EntityType.h"

class ResourceLoader
{
private:
	ResourceLoader();

	std::map<std::string, sf::Texture*> textureMap;
	std::map<std::string, sf::Font*> fontMap;

public:
	~ResourceLoader();

	// Singleton accessor
	static ResourceLoader& get()
	{
		static ResourceLoader r;
		return r;
	}

	sf::Texture* getTexture(const std::string& fileName);
	sf::Texture* getItemTexture(const ItemStack& item);
	sf::Texture* getEntityTexture(const EntityType type);
	sf::Font* getFont(const std::string& fileName);
};

