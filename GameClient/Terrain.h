#pragma once
//#include <Magnum/GL/Buffer.h>
//#include <Magnum/GL/Mesh.h>
//#include <Magnum/GL/Texture.h>
//#include <Magnum/GL/TextureFormat.h>
//#include <Magnum/Shaders/VertexColor.h>
//#include <Magnum/SceneGraph/Drawable.h>

//using namespace Magnum;

class Terrain// : public SceneGraph::Drawable3D
{
private:
	//Vector2i m_size;
	//struct TerrainVertex {
	//	Vector3 position;
	//	Color3 color;
	//};
	//TerrainVertex* m_vertices;
	//GL::Buffer m_vertexBuffer{NoCreate};
	//GL::Mesh m_mesh{NoCreate};

public:
	Terrain() = default;
	//Terrain(int width, int height, SceneGraph::AbstractObject3D& scene, SceneGraph::DrawableGroup3D& drawables);
	~Terrain();

private:
	void setSize(int x, int y);
};

