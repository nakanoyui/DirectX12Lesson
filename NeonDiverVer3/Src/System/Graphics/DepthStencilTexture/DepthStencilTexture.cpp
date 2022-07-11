#include "DepthStencilTexture.h"

void DepthStencilTexture::Create(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size)
{
	CreateBuffer(pGraphicsDevice,size);
	m_srvNumber = m_pGraphicsDevice->GetCBVSRVUAVHeap()->GetNextSrvNumber();
	m_pGraphicsDevice->GetCBVSRVUAVHeap()->CreateSRV(m_pDepthBuffer);
}

void DepthStencilTexture::Set(int descIndex)
{
	m_pGraphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex,m_pGraphicsDevice->GetCBVSRVUAVHeap()->GetSRVHandleForSRV(m_srvNumber));
}