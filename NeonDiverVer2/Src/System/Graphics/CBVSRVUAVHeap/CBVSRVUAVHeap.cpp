#include "CBVSRVUAVHeap.h"

CBVSRVUAVHeap::~CBVSRVUAVHeap()
{
	SafeRelease(m_pHeap);
}

void CBVSRVUAVHeap::SetGraphicsDevice(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCmdList)
{
	m_pDevice = pDevice;
	m_pCmdList = pCmdList;
}

void CBVSRVUAVHeap::Create(const Math::Vector3& useCount)
{
	if (!m_pDevice)return;
	if (!m_pCmdList)return;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NodeMask = 0;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = (int)useCount.x + (int)useCount.y + (int)useCount.z;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT result = m_pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pHeap));

	if (FAILED(result))assert(0&&"CBVSRVUAVHeap‚Ìì¬Ž¸”s");

	m_useCount = useCount;
	m_incrementSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CBVSRVUAVHeap::CreateSRV(ID3D12Resource* pBuffer)
{
	if (!m_pDevice)return;
	if (!m_pHeap)return;
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_pHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += ((UINT64)m_useCount.x + 1) * m_incrementSize + (UINT64)m_nextSrvDescriptorNum * m_incrementSize;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = pBuffer->GetDesc().Format;
	if (pBuffer->GetDesc().Format == DXGI_FORMAT_R32_TYPELESS)
	{
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	}
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	m_pDevice->CreateShaderResourceView(pBuffer, &srvDesc, handle);
	++m_nextSrvDescriptorNum;
}

void CBVSRVUAVHeap::CreateSRV(ID3D12Resource* pBuffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc)
{
	if (!m_pDevice)return;
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_pHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += ((UINT64)m_useCount.x + 1) * m_incrementSize + (UINT64)m_nextSrvDescriptorNum * m_incrementSize;
	m_pDevice->CreateShaderResourceView(pBuffer, &viewDesc, handle);
	++m_nextSrvDescriptorNum;
}

D3D12_GPU_DESCRIPTOR_HANDLE CBVSRVUAVHeap::GetSRVHandleForSRV(int number)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_pHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)m_incrementSize * ((UINT64)m_useCount.x + 1);
	handle.ptr += (UINT64)m_incrementSize * number;
	return handle;
}

void CBVSRVUAVHeap::Set()
{
	ID3D12DescriptorHeap* ppHeaps[] = { m_pHeap };
	m_pCmdList->SetDescriptorHeaps(1, ppHeaps);
}