#pragma once
#include "Global.h"

class C_Terrain
{
private:
	//sf::Image* m_heightmap;			// The heightmap image. Keep this loaded because the overword heightmap is often reloaded.
	u8* m_heightData;				// Pointer to the height data.
	Vec2<u16> m_heightDataSize;		// The size of the terrain model.
	std::string m_filename;			// The name of the image containing the most recent heightmap.

	// Rendering
	//bool m_shadersLoaded; // True if the terrain shaders have been loaded or not
	//GLuint m_vertexBufferObject, m_indexBufferObject, m_normalBuffer;
	//const unsigned int m_attribute_vertex = 0;
	//char *m_vertexSource, *m_fragmentSource; // Shader source
	//GLuint m_vertexShader, m_fragmentShader; // Shader handles
	//GLuint m_shaderProgram; // Shader program handle
	u32 m_numVertices; // The number of vertices in the terrain model
	u32 m_numIndices; // The number of indices in the terrain model
	u32 m_numTriangles; // The number of triangles in the terrain model

public:
	C_Terrain();
	~C_Terrain();

	/// Loads terrain from a section of a heightmap and generates a 3D model
	void loadTerrain(const std::string& filename, const Vec2<u16>& size, Vec2<u16> offset = Vec2<u16>(0, 0));
	
	/// Renders the terrain
	void render();

	/// returns the size of the vertex buffer
	const Vec2<u16>& getSize() const;
};

