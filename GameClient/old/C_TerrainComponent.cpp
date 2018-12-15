#include "C_TerrainComponent.h"

C_TerrainComponent::C_TerrainComponent()
{

}

C_TerrainComponent::~C_TerrainComponent()
{

}

const std::string& C_TerrainComponent::getDescription() const
{
	static const std::string desc = "A simple terrain model";
	return desc;
}

void C_TerrainComponent::render(sf::RenderWindow& window, const GameTime& gameTime)
{

}