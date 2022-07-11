#include "Mesh.h"

void Mesh::Create(GraphicsDevice* pGraphicsDevice, MeshData<TextureVertex>& meshData)
{
	m_pGraphicsDevice = pGraphicsDevice;
	ID3D12Device* device = pGraphicsDevice->GetDevice().Get();
	m_pCmdList = m_pGraphicsDevice->GetCmdList().Get();
	std::vector<TextureVertex>* vertices = meshData.GetVertices();
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
	resDesc.Width = sizeof(TextureVertex) * (*vertices).size();

	HRESULT result;
	result = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pVBuffer));

	m_vbView.BufferLocation = m_pVBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)resDesc.Width;
	m_vbView.StrideInBytes = sizeof(TextureVertex);
	resDesc.Width = sizeof(UINT) * (*indices).size();
	device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pIBuffer));
	m_ibView.BufferLocation = m_pIBuffer->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = (UINT)resDesc.Width;
	m_ibView.Format = DXGI_FORMAT_R32_UINT;

	TextureVertex* vbMap = nullptr;
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

void Mesh::DrawTex(UINT value)
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