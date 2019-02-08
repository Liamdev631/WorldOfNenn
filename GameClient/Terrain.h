//Represents a terrain, by storing a set of heights and normals at 2D locations
#include "Global.h"
#include <glm/glm.hpp>
#include <memory>
#include "shader.h"

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
	shared_ptr<Shader> m_shader;
	GLint mvp_location, vpos_location, vcol_location;

public:
	Terrain(int w2, int l2, shared_ptr<Shader> shader);
	~Terrain();

	const int& width();

	const int& length();

	//Sets the height at (x, z) to y
	void setHeight(int x, int z, float y);

	//Returns the height at (x, z)
	float getHeight(int x, int z);

	//Computes the normals, if they haven't been computed yet
	void computeNormals();

	//Returns the normal at (x, z)
	fvec3 getNormal(int x, int z);

	static unique_ptr<Terrain> loadTerrain(const char* filename, float height, shared_ptr<Shader> shader);

	void draw();
};
