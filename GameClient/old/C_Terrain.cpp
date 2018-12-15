#include "C_Terrain.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include "ShaderCompiler.hpp"

struct TerrainVertex
{
	Vec3f position;
	Vec3f normal;
};

C_Terrain::C_Terrain()
	: m_filename(""), m_heightData(nullptr), m_shadersLoaded(false), m_heightmap(new sf::Image())
{

}

C_Terrain::~C_Terrain()
{
	if (!m_shadersLoaded)
		return;
	glDeleteBuffers(1, &m_vertexBufferObject);
	glDeleteBuffers(1, &m_indexBufferObject);
}

void C_Terrain::loadTerrain(const std::string& filename, const Vec2<u16>& size, Vec2<u16> offset)
{
	if (!m_shadersLoaded)
	{
		//// Load the shaders if not already loaded
		m_vertexSource = readShaderSource("TerrainVertexShader.vert");
		m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_vertexShader, 1, (const char**)&m_vertexSource, 0);
		glCompileShader(m_vertexShader);
		free(m_vertexSource);

		m_fragmentSource = readShaderSource("TerrainFragmentShader.frag");
		m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_fragmentShader, 1, (const char**)&m_fragmentSource, 0);
		glCompileShader(m_fragmentShader);
		free(m_fragmentSource);

		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, m_vertexShader);
		glAttachShader(m_shaderProgram, m_fragmentShader);
		glBindAttribLocation(m_shaderProgram, m_attribute_vertex, "in_position");
		glLinkProgram(m_shaderProgram);


		//m_shader.loadFromFile("TerrainVertexShader.vert", "TerrainFragmentShader.frag");
		m_shadersLoaded = true;
	}

	// Reload the heightmap image if not already loaded
	if (m_filename != filename) {
		m_filename = filename;
		if (!m_heightmap->loadFromFile(filename)) {
			printf("Failed to heightmap (%s)!\n", filename.c_str());
			return;
		}
	}

	// Check for validity of bounds
	m_heightDataSize = size;
	if (m_heightDataSize.x + offset.x > (u16)m_heightmap->getSize().x)
		m_heightDataSize.x = m_heightmap->getSize().x - offset.x;
	if (m_heightDataSize.y + offset.y > (u16)m_heightmap->getSize().y)
		m_heightDataSize.y = m_heightmap->getSize().y - offset.y;

	// Extract the pixel data
	if (m_heightData)
		delete m_heightData;
	m_heightData = new u8[m_heightDataSize.x * m_heightDataSize.y];
	rgba* source = (rgba*)m_heightmap->getPixelsPtr();
	u8* destination = m_heightData;

	const size_t sourceRowSize = 4 * m_heightmap->getSize().x;
	const size_t destRowSize = m_heightDataSize.x;
	const auto cSize = m_heightDataSize;
	for (size_t y = 0; y < cSize.y; y++)
	{
		for (size_t x = 0; x < cSize.x; x++)
			destination[x] = source[x].r;
		source += sourceRowSize;
		destination += destRowSize;
	}

	// Calculate vertices
	m_numVertices = cSize.x * cSize.y;
	TerrainVertex* vertices = new TerrainVertex[m_numVertices];
	for (int y = 0; y < cSize.y; y++)
		for (int x = 0; x < cSize.x; x++)
		{
			const size_t index = y * cSize.x + x;
			TerrainVertex& v = vertices[index];
			v.position.x = (float)x;
			v.position.y = (float)y;
			v.position.z = m_heightData[index] * 0.05f;
			v.normal.x = 0;
			v.normal.y = 0;
			v.normal.z = 0;
		}

	// Calculate indices
	m_numIndices = (cSize.x - 1) * (cSize.y - 1) * 6;
	int* indices = new int[m_numIndices];
	int indexOffset = 0;
	for (int y = 0; y < cSize.y - 1; ++y)
		for (int x = 0; x < cSize.x - 1; ++x)
		{
			int start = y * cSize.y + x;
			indices[indexOffset++] = (short)start;
			indices[indexOffset++] = (short)(start + 1);
			indices[indexOffset++] = (short)(start + cSize.x);
			indices[indexOffset++] = (short)(start + 1);
			indices[indexOffset++] = (short)(start + 1 + cSize.x);
			indices[indexOffset++] = (short)(start + cSize.x);
		}

	// Calculate normals
	for (int i = 0; i < m_numIndices / 3; i++) {
		int index1 = indices[i * 3];
		int index2 = indices[i * 3 + 1];
		int index3 = indices[i * 3 + 2];
		Vec3f side1 = vertices[index1].position - vertices[index3].position;
		Vec3f side2 = vertices[index1].position - vertices[index2].position;
		Vec3f normal = cross(side1, side2);
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
		vertices[index3].normal += normal;
	}
	for (int i = 0; i < m_numVertices; i++) { // Normalize
		vertices[i].normal /= vertices[i].normal.length();
	}

	// Generate a buffer for the vertices
	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainVertex) * m_numVertices, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] vertices;

	// Generate a buffer for the indices
	glGenBuffers(1, &m_indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_numIndices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] indices;
}

void C_Terrain::render()
{
	glUseProgram(m_shaderProgram);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);

	GLintptr vertexPositionOffset = 0;
	GLintptr normalPositionOffset = sizeof(Vec3f);

	glVertexAttribPointer(vertexPositionOffset, 3, GL_FLOAT, false, 0, nullptr);

	//glUseProgram(m_shaderProgram);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glBindVertexArray(m_vertexBufferObject);
	//glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0); // Position
	//glVertexAttribPointer(1, 3, GL_FLOAT, true, 0, 0);	// Normal
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
	//glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);

	//glEnableVertexAttribArray(0); // Positions
	//glEnableVertexAttribArray(1); // Indices
	//glEnableVertexAttribArray(2); // Normals
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
	//glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	////glDrawArrays(GL_TRIANGLES, 0, m_heightDataSize.x * m_heightDataSize.y);
	//glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, (void*)0);
	//glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(0);
}

const Vec2<u16>& C_Terrain::getSize() const
{
	return m_heightDataSize;
}
