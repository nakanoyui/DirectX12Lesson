#include "RenderTarget.h"

RenderTarget::~RenderTarget()
{
	SafeRelease(m_pRTVHeap);
}

bool RenderTarget::Create(GraphicsDevice* pGraphicsDevice, std::vector<ID3D12Resource*>* pResources)
{
	m_pResources.resize((int)(*pResources).size());

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};

	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// レンダーターゲットとして扱う
	rtvHeapDesc.NodeMask = 0;
	rtvHeapDesc.NumDescriptors = (int)(*pResources).size();
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// 特に指定なし

	auto result = pGraphicsDevice->GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRTVHeap));

	if (FAILED(result))
	{
		return false;
	}

	// ビューの作成
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // (ガンマ補正)
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	int i = 0;
	for (auto& r : *pResources)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = m_pRTVHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHeapHandle.ptr += (UINT)i * pGraphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		pGraphicsDevice->GetDevice()->CreateRenderTargetView((*pResources)[i], &rtvDesc, rtvHeapHandle);
		m_pResources[i] = (*pResources)[i];
		++i;
	}
	m_pGraphicsDevice = pGraphicsDevice;
	return true;
}

void RenderTarget::SetCurrentResourceState(D3D12_RESOURCE_STATES state)
{
	m_currentResourceState = state;
}