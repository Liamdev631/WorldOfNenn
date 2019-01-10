#pragma once
#include <SFML/OpenGL.hpp>
#include <vector>
#include <glm.hpp>

class Terrain
{
private:
	typedef std::vector<glm::vec3> position_buffer;
	typedef std::vector<GLuint> index_buffer;

	GLuint m_glVertexBuffer;
	GLuint m_glIndexBuffer;

	glm::u16vec2 m_mapSize;
	float m_heightScale;

public:
	Terrain(int sizeX, int sizeY);
	~Terrain();
};

