#include "Terrain.h"
#include "SFML/Graphics.hpp"
#include <GL/glew.h>
#include "shader.h"

Terrain::Terrain(int w2, int l2, shared_ptr<Shader> shader) {
	w = w2;
	l = l2;

	hs = new float*[l];
	for (int i = 0; i < l; i++) {
		hs[i] = new float[w];
	}

	normals = new fvec3*[l];
	for (int i = 0; i < l; i++) {
		normals[i] = new fvec3[w];
	}

	computedNormals = false;

	mvp_location = glGetUniformLocation(shader->getHandle(), "MVP");
	vpos_location = glGetAttribLocation(shader->getHandle(), "vPos");
	vcol_location = glGetAttribLocation(shader->getHandle(), "vCol");
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));
}

Terrain::~Terrain() {
	for (int i = 0; i < l; i++) {
		delete[] hs[i];
	}
	delete[] hs;

	for (int i = 0; i < l; i++) {
		delete[] normals[i];
	}
	delete[] normals;
}

const int& Terrain::width() {
	return w;
}

const int& Terrain::length() {
	return l;
}

//Sets the height at (x, z) to y
void Terrain::setHeight(int x, int z, float y) {
	hs[z][x] = y;
	computedNormals = false;
}

//Returns the height at (x, z)
float Terrain::getHeight(int x, int z) {
	return hs[z][x];
}

//Computes the normals, if they haven't been computed yet
void Terrain::computeNormals()
{
	if (computedNormals)
		return;

	//Compute the rough version of the normals
	fvec3** normals2 = new fvec3*[l];
	for (int i = 0; i < l; i++)
		normals2[i] = new fvec3[w];

	for (int z = 0; z < l; z++)
	{
		for (int x = 0; x < w; x++)
		{
			fvec3 sum(0.0f, 0.0f, 0.0f);
			fvec3 out;
			if (z > 0)
				out = fvec3(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
			fvec3 in;
			if (z < l - 1)
				in = fvec3(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
			fvec3 left;
			if (x > 0)
				left = fvec3(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
			fvec3 right;
			if (x < w - 1)
				right = fvec3(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);

			if (x > 0 && z > 0)
				sum += normalize(cross(out, left));
			if (x > 0 && z < l - 1)
				sum += normalize(cross(left, in));
			if (x < w - 1 && z < l - 1)
				sum += normalize(cross(in, right));
			if (x < w - 1 && z > 0)
				sum += normalize(cross(right, out));

			normals2[z][x] = sum;
		}
	}

	//Smooth out the normals
	const float FALLOUT_RATIO = 0.5f;
	for (int z = 0; z < l; z++)
	{
		for (int x = 0; x < w; x++)
		{
			fvec3 sum = normals2[z][x];
			if (x > 0)
				sum += normals2[z][x - 1] * FALLOUT_RATIO;
			if (x < w - 1)
				sum += normals2[z][x + 1] * FALLOUT_RATIO;
			if (z > 0)
				sum += normals2[z - 1][x] * FALLOUT_RATIO;
			if (z < l - 1)
				sum += normals2[z + 1][x] * FALLOUT_RATIO;

			if (sum.length() <= std::numeric_limits<float>::min())
				sum = fvec3(0.0f, 1.0f, 0.0f);
			normals[z][x] = sum;
		}
	}

	for (int i = 0; i < l; i++)
		delete[] normals2[i];
	delete[] normals2;

	computedNormals = true;
}

//Returns the normal at (x, z)
fvec3 Terrain::getNormal(int x, int z)
{
	if (!computedNormals)
		computeNormals();
	return normals[z][x];
}

unique_ptr<Terrain> Terrain::loadTerrain(const char* filename, float height, shared_ptr<Shader> shader)
{
	//Image* image = loadBMP(filename);
	sf::Image image;
	image.loadFromFile(filename);
	auto t = make_unique<Terrain>(image.getSize().x, image.getSize().y, shader);
	int counter = 0;
	auto pixelsPtr = image.getPixelsPtr();
	for (int y = 0; y < (int)image.getSize().y; y++)
		for (int x = 0; x < (int)image.getSize().x; x++) {
			unsigned char color = (unsigned char)pixelsPtr[counter];
			counter += 4;
			float h = height * (color / 255.0f);
			t->setHeight(x, y, h);
		}

	t->computeNormals();
	return t;
}

void Terrain::draw()
{
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(0, 10, 0);
	glEnd();

	//	for (int z = 0; z < l - 1; z++)
	//	{
	//		glBegin(GL_TRIANGLES);
	//		for (int x = 0; x < w - 1; x++)
	//		{
	//			glColor3f(0, 0, 0);
	//			glColor3f(200, 0, 0);
	//			glColor3f(0, 300, 0);
	//		}
	//		glEnd();
	//	}
}