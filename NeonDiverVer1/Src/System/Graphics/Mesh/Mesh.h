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

	void Create(GraphicsDevice* pGraphicsDevice, MeshData<TextureVertex>& meshData);

	void Create(GraphicsDevice* pGraphicsDevice, const std::vector<MeshVertex>& vertices, const std::vector<MeshFace>& faces, const std::vector<MeshSubset>& subsets, bool bSkinMesh);

	void DrawTex(UINT value = 1);

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

	DirectX::BoundingBox	m_aabb;		// �����s���E�{�b�N�X
	DirectX::BoundingSphere m_bs;		// ���E��
	std::vector<Math::Vector3> m_positions;
	std::vector<MeshFace>	m_faces;

	bool						m_bSkinMesh = false;

	GraphicsDevice* m_pGraphicsDevice = nullptr;
	ID3D12GraphicsCommandList* m_pCmdList = nullptr;
};

struct Material
{
	//---------------------------------------
	// �ގ��f�[�^
	//---------------------------------------

	// ���O
	std::string					Name;

	// ��{�F
	std::shared_ptr<Texture>	BaseColorTex;				// ��{�F�e�N�X�`��
	Math::Vector4				BaseColor = { 1,1,1,1 };	// ��{�F�̃X�P�[�����O�W��(RGBA)

	// �������A�e��
	std::shared_ptr<Texture>	MetallicRoughnessTex;		// B:������ G:�e��
	float						Metallic = 0.0f;			// �������̃X�P�[�����O�W��
	float						Roughness = 1.0f;			// �e���̃X�P�[�����O�W��

	// ���Ȕ���
	std::shared_ptr<Texture>	EmissiveTex;				// ���Ȕ����e�N�X�`��
	Math::Vector3				Emissive = { 0,0,0 };		// ���Ȕ����̃X�P�[�����O�W��(RGB)

	// �@���}�b�v
	std::shared_ptr<Texture>	NormalTex;
};