#include "GPUParticleEmitter.h"

#include "Application/Application.h"

GPUParticleEmitter::GPUParticleEmitter()
	:m_useParticleOffset(0)
	, m_useParticleValue(0)
	, m_uavValue(0)
	, m_srvValue(0)
	, m_count(0)
{
	m_wpAssetManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetAssetManager();
}

GPUParticleEmitter::~GPUParticleEmitter()
{
}

void GPUParticleEmitter::Create(GPUParticleManager* pManager, UINT useParticleValue)
{
	m_useParticleValue = useParticleValue;

	// ƒrƒ…[‚Ì¶¬
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.FirstElement = pManager->GetNextParticleOffset();
	uavDesc.Buffer.NumElements = m_useParticleValue;
	uavDesc.Buffer.StructureByteStride = sizeof(ParticleData);

	m_uavValue = pManager->GetUAVNextOffset();
	pManager->CreateUAV(uavDesc);
	m_useParticleOffset = pManager->GetNextParticleOffset();
	pManager->IncrementParticleOffset(m_useParticleValue);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = uavDesc.Buffer.FirstElement;
	srvDesc.Buffer.NumElements = m_useParticleValue;
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleData);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	m_srvValue = pManager->GetDevice()->GetCBVSRVUAVHeap()->GetNextSrvNumber();
	pManager->GetDevice()->GetCBVSRVUAVHeap()->CreateSRV(pManager->GetUpdateParticleBuffer().Get(), srvDesc);

	pManager->GetParticleBuffer()->Map(0, nullptr, (void**)&m_upParticleData);
	pManager->GetUpdateParticleBuffer()->Map(0, nullptr, (void**)&m_upUpdateParticleData);
	pManager->GetEmitterParticleBuffer()->Map(0, nullptr, (void**)&m_pEmitterData);

	m_registValue = pManager->GetEmitterNextOffset();

	for (UINT i = 0; i < m_useParticleValue; ++i)
	{
		m_upParticleData[m_useParticleOffset + i].m_pos = { 100000,0,0 };
		m_upParticleData[m_useParticleOffset + i].m_index = m_registValue;
	}

	m_pManager = pManager;
}

void GPUParticleEmitter::Update(UINT emitteParticlePerSecpnd, UINT particleCyclePerFrame)
{
	bool bEmitteFlag = false;

	if (m_count >= particleCyclePerFrame)
	{
		bEmitteFlag = true;
		m_count = 0;
	}

	if (bEmitteFlag)
	{
		UINT count = 0;
		for (int i = 0; i < (int)m_useParticleValue; ++i)
		{
			if (count >= emitteParticlePerSecpnd)break;

			if (m_upParticleData[m_useParticleOffset + i].m_state == static_cast<int>(ParticleState::Dead))
			{
				m_upParticleData[m_useParticleOffset + i].m_state = static_cast<int>(ParticleState::Init);
				++count;
			}
			else
			{
				continue;
			}
		}
	}

	m_count++;
}

void GPUParticleEmitter::Draw(ComputePipeline* pComputePipeline, const Math::Matrix& mat, Texture* pTex)
{
	m_pManager->GetDevice()->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_pManager->GetDevice()->GetCBufferAllocater()->BindAndAttach(0, mat);
	Application::Instance().GetCameraManager()->Set3D(1);
	m_pManager->GetDevice()->GetCmdList()->SetGraphicsRootDescriptorTable(2, m_pManager->GetDevice()->GetCBVSRVUAVHeap()->GetSRVHandleForSRV(m_srvValue));

	if (!pTex)
	{
		m_wpAssetManager.lock()->GetTexture(Application::Instance().GetSceneManager()->GetCurrentScene()->GetJsonManager()->GetAssetJson().at("Texture").at("Particle").at("Default"))->Set(3);
	}
	else
	{
		pTex->Set(3);
	}

	m_wpAssetManager.lock()->GetMesh("Point")->DrawInstanced(m_useParticleValue);

	ID3D12DescriptorHeap* pHeap = m_pManager->GetHeap().Get();
	pComputePipeline->Set();
	m_pManager->GetDevice()->GetCmdList()->SetDescriptorHeaps(1, &pHeap);

	auto handle = pHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)m_pManager->GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * m_uavValue;
	m_pManager->GetDevice()->GetCmdList()->SetComputeRootDescriptorTable(0, handle);
	handle = pHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)m_pManager->GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 200 + (UINT64)m_pManager->GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * m_pManager->GetEmitterDataSRVValue();
	m_pManager->GetDevice()->GetCmdList()->SetComputeRootDescriptorTable(1, handle);

	m_pManager->GetDevice()->GetCmdList()->Dispatch(m_useParticleValue / 256, 1, 1);
}