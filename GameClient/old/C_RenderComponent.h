#pragma once
#include <string>
#include <Global.h>
#include <SFML/Graphics.hpp>

class C_RenderComponent
{
public:
	bool enabled;

protected:

public:
	C_RenderComponent();
	~C_RenderComponent();

	virtual const std::string& getDescription() const;
	virtual void render(sf::RenderWindow& window, const GameTime& gameTime) = 0;
	//virtual void render3D() = 0;
};