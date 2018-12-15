#pragma once
#include "C_RenderComponent.h"

struct VertexPositionColor
{
	float m_x;
	float m_y;
	float m_z;
};

static VertexPositionColor s_cubeVertices[] =
{
	{ -1.0f,  1.0f,  1.0f },
	{ 1.0f,  1.0f,  1.0f },
	{ -1.0f, -1.0f,  1.0f },
	{ 1.0f, -1.0f,  1.0f },
	{ -1.0f,  1.0f, -1.0f },
	{ 1.0f,  1.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f },
	{ 1.0f, -1.0f, -1.0f },
};

static const uint16_t s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};


class C_TerrainComponent : public Render::C_RenderComponent
{
public:
	C_TerrainComponent();
	~C_TerrainComponent();

	const std::string& getDescription() const override;
	void render(sf::RenderWindow& window, const GameTime& gameTime) override;

private:

};
