#pragma once

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