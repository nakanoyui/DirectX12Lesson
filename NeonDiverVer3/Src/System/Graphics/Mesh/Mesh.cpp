#include "Mesh.h"

void Mesh::Create(GraphicsDevice* pGraphicsDevice, const std::vector<MeshVertex>& vertices, const std::vector<MeshFace>& faces, const std::vector<MeshSubset>& subsets, bool bSkinMesh)
{
	m_pGraphicsDevice = pGraphicsDevice;
	ID3D12Device* device = m_pGraphicsDevice->GetDevice().Get();
	m_pCmdList = m_pGraphicsDevice->GetCmdList().Get();

	m_subsets = subsets;

	if ((int)vertices.size() == 0)assert(0 && "頂点が1つもありません");

	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = static_cast<UINT>(sizeof(MeshVertex) * (vertices).size());
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = DXGI_FORMAT_UNKNOWN;
	resdesc.SampleDesc.Count = 1;
	resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = GraphicsDevice::Instance().GetDevice()->CreateCommittedResource(
		&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pVBuffer));

	if (FAILED(result))assert(0 && "頂点バッファー作成失敗");

	m_vbView.BufferLocation = m_pVBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)resdesc.Width;
	m_vbView.StrideInBytes = sizeof(MeshVertex);
	resdesc.Width = static_cast <UINT>(sizeof(MeshFace) * (faces).size());
	result = device->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pIBuffer));
	if (FAILED(result))assert(0 && "インデックスバッファー作成失敗");
	m_ibView.BufferLocation = m_pIBuffer->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = (UINT)resdesc.Width;
	m_ibView.Format = DXGI_FORMAT_R32_UINT;

	m_positions.resize(vertices.size());
	for (UINT i = 0; i < m_positions.size(); ++i)
	{
		m_positions[i] = vertices[i].pos;
	}

	// AA境界データ作成
	DirectX::BoundingBox::CreateFromPoints(m_aabb, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));
	// 境界球データ作成
	DirectX::BoundingSphere::CreateFromPoints(m_bs, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));

	MeshVertex* vbMap = nullptr;
	MeshFace* ibMap = nullptr;
	{
		m_pVBuffer->Map(0, nullptr, (void**)&vbMap);
		for (int i = 0; i < (int)(vertices).size(); ++i)
		{
			vbMap[i] = (vertices)[i];
		}
		m_pVBuffer->Unmap(0, nullptr);
	}
	{
		m_pIBuffer->Map(0, nullptr, (void**)&ibMap);
		for (int i = 0; i < (int)(faces).size(); ++i)
		{
			ibMap[i] = (faces)[i];
		}
		m_pIBuffer->Unmap(0, nullptr);
	}

	m_bSkinMesh = bSkinMesh;
}

void Mesh::DrawInstanced(UINT value)
{
	m_pCmdList->IASetVertexBuffers(0, 1, &m_vbView);
	m_pCmdList->IASetIndexBuffer(&m_ibView);
	m_pCmdList->DrawIndexedInstanced(m_indicesCount, value, 0, 0, 0);
}

void Mesh::DrawSubset(int subsetNo)const
{
	m_pCmdList->IASetVertexBuffers(0, 1, &m_vbView);
	m_pCmdList->IASetIndexBuffer(&m_ibView);
	m_pCmdList->DrawIndexedInstanced(m_subsets[subsetNo].FaceCount * 3, 1, m_subsets[subsetNo].FaceStart * 3, 0, 0);
}