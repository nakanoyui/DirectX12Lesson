#include "MeshCreater.h"

void MeshCreater::Create2DQuad(MeshData<TextureVertex>& meshData, Math::Vector2 size)
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

void MeshCreater::CreateLineSphere(float size, float vertexCount, const Math::Color& color, MeshData<LineObject>& meshData)
{
	std::vector<LineObject>* vertices = meshData.GetVertices();
	std::vector<unsigned int>* indices = meshData.GetIndices();

	for (int i = 0; i < vertexCount; ++i)
	{
		Math::Vector3 pos;
		pos.x = size * sinf((2 * static_cast<float>(M_PI) / vertexCount) * i) / 2;
		pos.y = 0;
		pos.z = size * cosf((2 * static_cast<float>(M_PI) / vertexCount) * i) / 2;
		unsigned int col = color.RGBA().v;
		vertices->push_back({ pos,col });

		indices->push_back(i);
		int index = (i >= vertexCount - 1) ? 0 : i + 1;
		indices->push_back(index);
	}

	int indicesSize = static_cast<int>(indices->size()) / 2;

	for (int i = 0; i < vertexCount; ++i)
	{
		Math::Vector3 pos;
		pos.x = size * sinf((2 * static_cast<float>(M_PI) / vertexCount) * i) / 2;
		pos.y = size * cosf((2 * static_cast<float>(M_PI) / vertexCount) * i) / 2;
		pos.z = 0;
		unsigned int col = color.RGBA().v;
		vertices->push_back({ pos,col });

		indices->push_back(i + indicesSize);
		int index = (i >= vertexCount - 1) ? 0 : i + 1;
		indices->push_back(index + indicesSize);
	}

	indicesSize = static_cast<int>(indices->size()) / 2;

	for (int i = 0; i < vertexCount; ++i)
	{
		Math::Vector3 pos;
		pos.x = 0;
		pos.y = size * cosf((2 * static_cast<float>(M_PI) / vertexCount) * i) / 2;
		pos.z = size * sinf((2 * static_cast<float>(M_PI) / vertexCount) * i) / 2;
		unsigned int col = color.RGBA().v;
		vertices->push_back({ pos,col });

		indices->push_back(i + indicesSize);
		int index = (i >= vertexCount - 1) ? 0 : i + 1;
		indices->push_back(index + indicesSize);
	}
}

void MeshCreater::CreateLineCube(Math::Vector3 size, const Math::Color& color, MeshData<LineObject>& meshData)
{
	std::vector<LineObject>* vertices = meshData.GetVertices();
	std::vector<unsigned int>* indices = meshData.GetIndices();

	unsigned int col = color.RGBA().v;

	//?O
	vertices->push_back({ Math::Vector3(-size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),col });
	vertices->push_back({ Math::Vector3(size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f), col });
	vertices->push_back({ Math::Vector3(size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f), col });
	vertices->push_back({ Math::Vector3(-size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),col });
	//??
	vertices->push_back({ Math::Vector3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f), col });
	vertices->push_back({ Math::Vector3(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),col });
	vertices->push_back({ Math::Vector3(-size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),col });
	vertices->push_back({ Math::Vector3(size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f), col });

	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(1);
	indices->push_back(2);
	indices->push_back(2);
	indices->push_back(3);

	indices->push_back(1);
	indices->push_back(4);
	indices->push_back(4);
	indices->push_back(7);
	indices->push_back(7);
	indices->push_back(2);

	indices->push_back(4);
	indices->push_back(5);
	indices->push_back(5);
	indices->push_back(6);
	indices->push_back(6);
	indices->push_back(7);

	indices->push_back(5);
	indices->push_back(0);
	indices->push_back(0);
	indices->push_back(3);
	indices->push_back(3);
	indices->push_back(6);
}