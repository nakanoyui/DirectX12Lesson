#pragma once

#include "../Mesh/MeshData/MeshData.h"

struct MeshVertex
{
	Math::Vector3 pos;
	Math::Vector2 uv;
	Math::Vector3 normal;
	UINT	color = 0xFFFFFFFF;
	Math::Vector3 tangent;

	std::array<short, 4> SkinIndexList;
	std::array<float, 4> SkinWeightList;
};

struct MeshFace
{
	UINT Idx[3];
};

struct MeshSubset
{
	UINT		MaterialNo = 0;

	UINT		FaceStart = 0;
	UINT		FaceCount = 0;
};

class Mesh
{
public:
	Mesh() = default;
	~Mesh() { Clear(); }

	template<typename T>
	void Create(GraphicsDevice* pGraphicsDevice, MeshData<T>& meshData);

	void Create(GraphicsDevice* pGraphicsDevice, const std::vector<MeshVertex>& vertices, const std::vector<MeshFace>& faces, const std::vector<MeshSubset>& subsets, bool bSkinMesh);

	void DrawInstanced(UINT value = 1);

	void DrawSubset(int subsetNo)const;

	unsigned int GetVertexDataSize() { return m_vertexDataSize; }
	std::vector<MeshVertex>* GetVertices() { return &m_vertices; }

	const std::vector<MeshSubset>& GetSubsets() const { return m_subsets; }

	const std::vector<Math::Vector3>& GetVertexPositions() const { return m_positions; }

	const std::vector<MeshFace>& GetFaces() const { return m_faces; }

	const DirectX::BoundingBox& GetBoundingBox() const { return m_aabb; }

	const DirectX::BoundingSphere& GetBoundingSphere() const { return m_bs; }

	bool GetIsSkinMesh()const { return m_bSkinMesh; }

	void Clear()
	{
		SafeRelease(m_pVBuffer);
		SafeRelease(m_pIBuffer);
		m_vertices.clear();
		m_subsets.clear();
		m_positions.clear();
		m_faces.clear();
	}

private:
	ID3D12Resource* m_pVBuffer = nullptr;
	ID3D12Resource* m_pIBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW	m_vbView;
	D3D12_INDEX_BUFFER_VIEW		m_ibView;
	UINT						m_indicesCount = 0;

	unsigned int	m_vertexDataSize;
	std::vector<MeshVertex>	m_vertices;

	std::vector<MeshSubset>	m_subsets;

	DirectX::BoundingBox	m_aabb;		// 軸平行境界ボックス
	DirectX::BoundingSphere m_bs;		// 境界球
	std::vector<Math::Vector3> m_positions;
	std::vector<MeshFace>	m_faces;

	bool						m_bSkinMesh = false;

	GraphicsDevice* m_pGraphicsDevice = nullptr;
	ID3D12GraphicsCommandList* m_pCmdList = nullptr;
};

struct Material
{
	//---------------------------------------
	// 材質データ
	//---------------------------------------

	// 名前
	std::string					Name;

	// 基本色
	std::shared_ptr<Texture>	BaseColorTex;				// 基本色テクスチャ
	Math::Vector4				BaseColor = { 1,1,1,1 };	// 基本色のスケーリング係数(RGBA)

	// 金属性、粗さ
	std::shared_ptr<Texture>	MetallicRoughnessTex;		// B:金属製 G:粗さ
	float						Metallic = 0.0f;			// 金属性のスケーリング係数
	float						Roughness = 1.0f;			// 粗さのスケーリング係数

	// 自己発光
	std::shared_ptr<Texture>	EmissiveTex;				// 自己発光テクスチャ
	Math::Vector3				Emissive = { 0,0,0 };		// 自己発光のスケーリング係数(RGB)

	// 法線マップ
	std::shared_ptr<Texture>	NormalTex;
};

template<typename T>
inline void Mesh::Create(GraphicsDevice* pGraphicsDevice, MeshData<T>& meshData)
{
	m_pGraphicsDevice = pGraphicsDevice;
	ID3D12Device* device = pGraphicsDevice->GetDevice().Get();
	m_pCmdList = m_pGraphicsDevice->GetCmdList().Get();
	std::vector<T>* vertices = meshData.GetVertices();
	std::vector<UINT>* indices = meshData.GetIndices();

	if ((int)vertices->size() == 0)return;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Width = sizeof(T) * (*vertices).size();

	HRESULT result;
	result = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pVBuffer));

	m_vbView.BufferLocation = m_pVBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)resDesc.Width;
	m_vbView.StrideInBytes = sizeof(T);
	resDesc.Width = sizeof(UINT) * (*indices).size();
	device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pIBuffer));
	m_ibView.BufferLocation = m_pIBuffer->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = (UINT)resDesc.Width;
	m_ibView.Format = DXGI_FORMAT_R32_UINT;

	T* vbMap = nullptr;
	UINT* ibMap = nullptr;
	m_pVBuffer->Map(0, nullptr, (void**)&vbMap);
	m_pIBuffer->Map(0, nullptr, (void**)&ibMap);
	for (int i = 0; i < (int)(*vertices).size(); ++i)
	{
		vbMap[i] = (*vertices)[i];
	}
	for (int i = 0; i < (int)(*indices).size(); ++i)
	{
		ibMap[i] = (*indices)[i];
	}
	m_pVBuffer->Unmap(0, nullptr);
	m_pIBuffer->Unmap(0, nullptr);

	m_indicesCount = (UINT)(*indices).size();
}