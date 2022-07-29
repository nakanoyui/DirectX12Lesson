#pragma once

struct Vertex
{
	Math::Vector3 m_point;

	Vertex() : m_point() {}
	Vertex(const Math::Vector3& p) :m_point(p) {}
};

struct TextureVertex
{
	Math::Vector3 pos;
	Math::Vector2 uv;
};

struct ModelVertex
{
	Math::Vector3 pos;
	Math::Vector2 uv;
	Math::Vector3 normal;
	UINT	color = 0xFFFFFFFF;
	Math::Vector3 tangent;

	std::array<short, 4> SkinIndexList;
	std::array<float, 4> SkinWeightList;
};

//シェーダーに送るための頂点情報構造体
template<typename T>
class MeshData
{
private:
	unsigned int vertexDataSize;
	std::vector<T> vertices;
	std::vector<unsigned int> indices;
public:
	MeshData();
	unsigned int GetVertexDataSize() { return vertexDataSize; }
	std::vector<T>* GetVertices() { return &vertices; }
	std::vector<unsigned int>* GetIndices() { return &indices; }
	void Clear()
	{
		vertices.clear();
		indices.clear();
	}

};
template<typename T>
inline MeshData<T>::MeshData() :vertexDataSize(sizeof(T))
{
	Clear();
}