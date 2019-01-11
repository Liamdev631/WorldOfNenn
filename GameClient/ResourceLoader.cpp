#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{

}

ResourceLoader::~ResourceLoader()
{

}

sf::Texture* ResourceLoader::getTexture(const std::string& fileName)
{
	auto tex = new sf::Texture();
	if (!tex->loadFromFile(fileName))
		tex->loadFromFile("assets/default.png");
	return tex;
}

sf::Texture* ResourceLoader::getItemTexture(const ItemStack& item)
{
	std::string filename = "assets/graphics/items/item_" + std::to_string(item.type) + ".png";
	return getTexture(filename);
}

sf::Texture* ResourceLoader::getEntityTexture(const EntityType& type)
{
	std::string filename = "assets/graphics/entities/entity_" + std::to_string(type) + ".png";
	return getTexture(filename);
}

sf::Font* ResourceLoader::getFont(const std::string& fileName)
{
	auto font = new sf::Font();
	font->loadFromFile(fileName);
	return font;
}
