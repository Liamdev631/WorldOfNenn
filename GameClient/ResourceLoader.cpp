#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{

}

ResourceLoader::~ResourceLoader()
{
	//for (auto iter = spriteMap.begin(); iter != spriteMap.end(); iter++)
	//	delete (*iter).second.first;
}

Sprite* ResourceLoader::getSprite(const std::string& fileName)
{
	//auto sprite = spriteMap.find(fileName);
	//if (sprite == spriteMap.end())
	//{
	//	// Load the sprite
	//	auto& newSprite = spriteMap[fileName];
	//	if (newSprite.texture.loadFromFile(fileName))
	//	{
	//		printf("Loaded sprite: %s\n", fileName.c_str());
	//		newSprite.sprite.setTexture(newSprite.texture);
	//		return &newSprite.sprite;
	//	}
	//	else
	//	{
	//		printf("Failed loaded sprite: %s\n", fileName.c_str());
	//		return getSprite("assets/default.png");
	//	}
	//}
	//return &sprite->second.sprite;
	auto sprite = new Sprite();
	if (!sprite->texture.loadFromFile(fileName))
		sprite->texture.loadFromFile("assets/default.png");
	sprite->sprite.setTexture(sprite->texture);
	return sprite;
}

sf::Sprite& ResourceLoader::getItemSprite(const ItemStack& item)
{
	std::string filename = "assets/graphics/items/item_" + std::to_string(item.type) + ".png";
	return getSprite(filename)->sprite;
}

sf::Font* ResourceLoader::getFont(const std::string& fileName)
{
	//auto font = fontMap.find(fileName);
	//if (font == fontMap.end())
	//{
	//	// Load the sprite
	//	//spriteMap[fileName]
	//	auto& newFont = fontMap[fileName];
	//	if (!newFont.loadFromFile(fileName))
	//		printf("Failed loading font %s\n", fileName.c_str());
	//	return &newFont;
	//}
	//return &font->second;
	auto font = new sf::Font();
	font->loadFromFile(fileName);
	return font;
}
