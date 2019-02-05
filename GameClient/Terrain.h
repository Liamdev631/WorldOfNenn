//Represents a terrain, by storing a set of heights and normals at 2D locations
#include "Global.h"
#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

using namespace std;
using namespace glm;

class Terrain
{
private:
	int w; //Width
	int l; //Length
	float** hs; //Heights
	fvec3** normals;
	bool computedNormals; //Whether normals is up-to-date
public:
	Terrain(int w2, int l2) {
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
	}

	~Terrain() {
		for (int i = 0; i < l; i++) {
			delete[] hs[i];
		}
		delete[] hs;

		for (int i = 0; i < l; i++) {
			delete[] normals[i];
		}
		delete[] normals;
	}

	int width() {
		return w;
	}

	int length() {
		return l;
	}

	//Sets the height at (x, z) to y
	void setHeight(int x, int z, float y) {
		hs[z][x] = y;
		computedNormals = false;
	}

	//Returns the height at (x, z)
	float getHeight(int x, int z) {
		return hs[z][x];
	}

	//Computes the normals, if they haven't been computed yet
	void computeNormals()
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
	fvec3 getNormal(int x, int z)
	{
		if (!computedNormals)
			computeNormals();
		return normals[z][x];
	}

	static unique_ptr<Terrain> loadTerrain(const char* filename, float height)
	{
		//Image* image = loadBMP(filename);
		sf::Image image;
		image.loadFromFile(filename);
		auto t = make_unique<Terrain>(image.getSize().x, image.getSize().y);
		auto pixelsPtr = image.getPixelsPtr();
		for (int y = 0; y < (int)image.getSize().y; y++)
			for (int x = 0; x < (int)image.getSize().x; x++) {
				unsigned char color = (unsigned char)pixelsPtr[4 * (y * image.getSize().x + x)];
				float h = height * ((color / 255.0f) - 0.5f);
				t->setHeight(x, y, h);
			}

		t->computeNormals();
		return t;
	}
};
