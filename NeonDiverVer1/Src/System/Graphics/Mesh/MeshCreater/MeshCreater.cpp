#include "MeshCreater.h"

void MeshCreater::Create2DQuad(MeshData<TextureVertex>& meshData,Math::Vector2 size)
{
	std::vector<TextureVertex>* vertices = meshData.GetVertices();
	std::vector<unsigned int>* indices = meshData.GetIndices();

	vertices->push_back({ Math::Vector3(-size.x / 2.0f,-size.y / 2.0f,0),Math::Vector2(0,1) });
	vertices->push_back({ Math::Vector3(size.x / 2.0f,-size.y / 2.0f,0),Math::Vector2(1,1) });
	vertices->push_back({ Math::Vector3(size.x / 2.0f,size.y / 2.0f,0) ,Math::Vector2(1,0) });
	vertices->push_back({ Math::Vector3(-size.x / 2.0f,size.y / 2.0f,0) ,Math::Vector2(0,0) });

	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);
	indices->push_back(2);
	indices->push_back(3);
	indices->push_back(0);
}