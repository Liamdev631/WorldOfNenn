#include "Terrain.h"
//#include <Corrade/Containers/ArrayView.h>

//using namespace Math::Literals;

//Terrain::Terrain(int width, int height, SceneGraph::AbstractObject3D& scene, SceneGraph::DrawableGroup3D& drawables)
//	: SceneGraph::Drawable3D(scene, &drawables)
//{
//	setSize(width, height);
//}

void Terrain::setSize(int width, int height)
{
	//m_vertices = new TerrainVertex[(width + 1) * (height + 1)];
	//int index = 0;
	//for (int y = 0; y < height; y++)
	//	for (int x = 0; x < width; x++)
	//	{
	//		auto& vertex = m_vertices[index++];
	//		vertex.position = Vector3((float)x, (float)y, 0);
	//		vertex.color = 0xffaab2_rgbf;
	//	}

	//auto verticesView = Corrade::Containers::ArrayView<TerrainVertex>(m_vertices, index);
	//m_vertexBuffer = GL::Buffer();
	//m_vertexBuffer.setData(verticesView, GL::BufferUsage::StaticDraw);
	//m_mesh = GL::Mesh(GL::MeshPrimitive::Triangles);
	//m_mesh.setCount(3)
	//	.addVertexBuffer(m_vertexBuffer, 0,
	//		Shaders::VertexColor3D::Position(),
	//		Shaders::VertexColor3D::Color3());
}

Terrain::~Terrain()
{

}
