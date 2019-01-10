#pragma once
#include <SFML/OpenGL.hpp>

inline void DeleteVertexBuffer(GLuint& vboID)
{
	if (vboID != 0)
	{
		//glDeleteBuffersARB(1, &vboID);
		vboID = 0;
	}
}

inline void CreateVertexBuffer(GLuint& vboID)
{
	// Make sure we don't loose the reference to the previous VBO if there is one
	DeleteVertexBuffer(vboID);
	//glGenBuffersARB(1, &vboID);
}