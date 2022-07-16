#include "CBufferAllocater.h"

CBufferAllocater::~CBufferAllocater()
{
	m_pBuffer->Unmap(0, {});
	SafeRelease(m_pBuffer);
}

void CBufferAllocater::SetGraphicsDevice(GraphicsDevice* pGraphicsDevice)
{
	m_pGraphicsDevice = pGraphicsDevice;
}

void CBufferAllocater::SetHeap(CBVSRVUAVHeap* pHeap)
{
	m_pHeap = pHeap;
}

void CBufferAllocater::Create()
{
	if (!m_pGraphicsDevice)return;
	if (!m_pHeap)return;

	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Width = (UINT64)((1 + 0xff) & ~0xff) * (int)m_pHeap->GetUseCount().x;
	HRESULT result = m_pGraphicsDevice->GetDevice()->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pBuffer));

	if (FAILED(result))assert(0 && "CBufferAllocater‚Ìì¬Ž¸”s");

	m_pBuffer->Map(0, {}, (void**)&m_pMappedBuffer);
}

void CBufferAllocater::ResetCurrentUseNumber()
{
	m_currentUseNumber = 0;
}

void CBufferAllocater::BindAndAttachData(int descIndex, const void* pData, int size)
{
	if (!m_pHeap)return;

	int sizeAligned = (size + 0xff) & ~0xff;
	int numRequired = sizeAligned / 0x100;

	if (m_currentUseNumber + numRequired > (int)m_pHeap->GetUseCount().x)return;

	int top = m_currentUseNumber;

	memcpy(m_pMappedBuffer + top, pData, size);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc = {};
	cbDesc.BufferLocation = m_pBuffer->GetGPUVirtualAddress() + (UINT64)top * 0x100;
	cbDesc.SizeInBytes = sizeAligned;

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = m_pHeap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	cpuHandle.ptr += (UINT64)m_pGraphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * m_currentUseNumber;
	m_pGraphicsDevice->GetDevice()->CreateConstantBufferView(&cbDesc, cpuHandle);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = m_pHeap->GetHeap()->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += (UINT64)m_pGraphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * m_currentUseNumber;

	m_pGraphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, gpuHandle);

	m_currentUseNumber += numRequired;
}