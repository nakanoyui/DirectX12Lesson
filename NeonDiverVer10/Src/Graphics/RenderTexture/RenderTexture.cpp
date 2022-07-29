#include "RenderTexture.h"

#include "../CBVSRVUAVHeap/CBVSRVUAVHeap.h"

RenderTexture::~RenderTexture()
{
}

void RenderTexture::Prepare()
{
	// SRVからRTVに変更
	if (m_currentResourceState != D3D12_RESOURCE_STATE_RENDER_TARGET)
	{
		m_currentResourceState = D3D12_RESOURCE_STATE_RENDER_TARGET;
		m_pGraphicsDevice->SetResourceBarrier(m_pTexBuffer.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
}

void RenderTexture::Create(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size, const Math::Vector4& color, bool bMeshCreate)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = pGraphicsDevice->GetCurrentFrameBuffer()->GetDesc();
	resDesc.Width = (UINT)size.x;
	resDesc.Height = (UINT)size.y;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	clearValue.Color[0] = (color.x > 1) ? color.x / 255.0f : color.x;
	clearValue.Color[1] = (color.y > 1) ? color.y / 255.0f : color.y;
	clearValue.Color[2] = (color.z > 1) ? color.z / 255.0f : color.z;
	clearValue.Color[3] = (color.w > 1) ? color.w / 255.0f : color.w;

	pGraphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&m_pTexBuffer));

	m_pResources.resize(1);
	// ヒープ生成
	HRESULT result;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 1;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	result = pGraphicsDevice->GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRTVHeap));
	// ビューの生成
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = m_pRTVHeap->GetCPUDescriptorHandleForHeapStart();
	pGraphicsDevice->GetDevice()->CreateRenderTargetView(m_pTexBuffer.Get(), &rtvDesc, rtvHeapHandle);
	m_pResources[0] = m_pTexBuffer.Get();

	m_srvNumber = pGraphicsDevice->GetCBVSRVUAVHeap()->GetNextSrvNumber();
	pGraphicsDevice->GetCBVSRVUAVHeap()->CreateSRV(m_pTexBuffer.Get());

	if (bMeshCreate)
	{
		MeshData<TextureVertex>	texVert;
		MeshCreater::Create2DQuad(texVert, { size.x ,size.y });
		m_spMesh = std::make_shared<Mesh>();
		m_spMesh->Create(&GraphicsDevice::Instance(), texVert);
	}

	m_pGraphicsDevice = pGraphicsDevice;
}

void RenderTexture::Set(int descIndex)
{
	if (m_currentResourceState == D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)EndDraw();
	m_pGraphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, m_pGraphicsDevice->GetCBVSRVUAVHeap()->GetSRVHandleForSRV(m_srvNumber));
}

void RenderTexture::EndDraw()
{
	// RTVからSRVに変更してシェーダーにセット
	m_currentResourceState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	m_pGraphicsDevice->SetResourceBarrier(m_pTexBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}