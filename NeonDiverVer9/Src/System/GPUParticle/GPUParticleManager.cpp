#include "GPUParticleManager.h"

GPUParticleManager::GPUParticleManager(GraphicsDevice* pGraphicsDevice)
	:MaxParticleValue(256000)
	, m_nextParticleOffset(0)
	, MaxEmitterValue(400)
	, m_uavNextOffset(0)
	, m_srvNextOffset(0)
	, m_emitterDataSRVValue(0)
	, m_emitterNextOffset(0)
	, m_pGraphicsDevice(pGraphicsDevice)
{
	HRESULT hr;
	// リソースの生成
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.CreationNodeMask = 1;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Alignment = 0;
	resDesc.DepthOrArraySize = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Height = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc = { 1,0 };
	resDesc.Width = sizeof(ParticleData) * MaxParticleValue;

	hr = m_pGraphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, IID_PPV_ARGS(&m_pParticleBuffer));

	if (FAILED(hr))assert(0 && "GPUパーティクル用のバッファー作成に失敗しました");

	// SRV用バッファー生成
	resDesc.Flags = {};
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Width = sizeof(ParticleData) * MaxParticleValue;

	hr = m_pGraphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pUpdateParticleBuffer));

	if (FAILED(hr))assert(0 && "GPUパーティクル用の更新バッファー作成に失敗しました");

	// ヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = MaxEmitterValue;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	hr = m_pGraphicsDevice->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pHeap));

	if (FAILED(hr))assert(0 && "GPUパーティクル用のヒープ作成に失敗しました");

	// エミッターデータ作成
	resDesc.Width = sizeof(EmitterData)* MaxEmitterValue;

	m_pGraphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pEmitterParticleBuffer));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = MaxEmitterValue;
	srvDesc.Buffer.StructureByteStride = sizeof(EmitterData);
	m_emitterDataSRVValue = GetSRVNextOffset();
	CreateSRV(m_pEmitterParticleBuffer.Get(), srvDesc);
}

GPUParticleManager::~GPUParticleManager()
{
}

void GPUParticleManager::CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& viewDesc)
{
	// ヒープのハンドルを進めて新たいUAVを登録
	auto handle = m_pHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)m_pGraphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * m_uavNextOffset;
	m_pGraphicsDevice->GetDevice()->CreateUnorderedAccessView(m_pParticleBuffer.Get(), nullptr, &viewDesc, handle);

	++m_uavNextOffset;
}

void GPUParticleManager::CreateSRV(ID3D12Resource* pBuffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc)
{
	// ヒープのハンドルを進めて新たにSRVを登録
	auto handle = m_pHeap->GetCPUDescriptorHandleForHeapStart();
	UINT incrementSize = m_pGraphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	handle.ptr += (UINT64)incrementSize * 200 + (UINT64)incrementSize * m_srvNextOffset;
	m_pGraphicsDevice->GetDevice()->CreateShaderResourceView(pBuffer, &viewDesc, handle);

	++m_srvNextOffset;
}

void GPUParticleManager::Update()
{
	ParticleData* particleData = nullptr;
	ParticleData* updateParticleData = nullptr;

	m_pParticleBuffer->Map(0, nullptr, (void**)&particleData);
	m_pUpdateParticleBuffer->Map(0, nullptr, (void**)&updateParticleData);

	// 0 ~ 現在使っているバッファ領域までをコピー
	memcpy(updateParticleData, particleData, sizeof(ParticleData) * m_nextParticleOffset);
}

void GPUParticleManager::Dispatch(ComputePipeline* pShader)
{
}