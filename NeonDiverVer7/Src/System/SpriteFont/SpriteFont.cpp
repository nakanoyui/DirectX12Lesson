#include "SpriteFont.h"

void SpriteFont::Init(GraphicsDevice*pGraphicsDevice)
{
	m_pGraphicsDevice = pGraphicsDevice;

	m_upGraphicsMemory = std::make_unique<DirectX::GraphicsMemory>(m_pGraphicsDevice->GetDevice().Get());
	DirectX::ResourceUploadBatch resUploadBatch(m_pGraphicsDevice->GetDevice().Get());
	resUploadBatch.Begin();
	DirectX::RenderTargetState rtState(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	DirectX::SpriteBatchPipelineStateDescription pd(rtState);
	m_upSpriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pGraphicsDevice->GetDevice().Get(),resUploadBatch,pd);

	m_pHeapForSpriteFont = CreateOneDescriptorHeapSRV();
	m_upSpriteFont = std::make_unique<DirectX::SpriteFont>(
		m_pGraphicsDevice->GetDevice().Get(),
		resUploadBatch,
		L"Asset/SpriteFont/SpriteFont.dat",
		m_pHeapForSpriteFont->GetCPUDescriptorHandleForHeapStart(),
		m_pHeapForSpriteFont->GetGPUDescriptorHandleForHeapStart());

	auto future = resUploadBatch.End(m_pGraphicsDevice->GetCmdQueue().Get());
	m_pGraphicsDevice->WaitForCommandQueue();
	future.wait();
	m_upSpriteBatch->SetViewport(m_pGraphicsDevice->GetViewport());
}

void SpriteFont::Begin()
{
	m_pGraphicsDevice->GetCmdList()->SetDescriptorHeaps(1, m_pHeapForSpriteFont.GetAddressOf());
	m_upSpriteBatch->Begin(m_pGraphicsDevice->GetCmdList().Get());
}

void SpriteFont::End()
{
	m_upSpriteBatch->End();
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SpriteFont::CreateOneDescriptorHeapSRV()
{
	ComPtr<ID3D12DescriptorHeap>ret;
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	GraphicsDevice::Instance().GetDevice()->CreateDescriptorHeap(&desc,IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));

	return ret;
}