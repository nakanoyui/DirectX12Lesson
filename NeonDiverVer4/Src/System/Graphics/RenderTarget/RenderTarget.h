#pragma once

class RenderTarget
{
public:
	RenderTarget() = default;
	virtual ~RenderTarget();

	ID3D12DescriptorHeap* GetHeap() { return m_pRTVHeap; }
	bool Create(GraphicsDevice* pGraphicsDevice, std::vector<ID3D12Resource*>* pResources);
	bool Create(GraphicsDevice* pGraphicsDevice, ID3D12Resource* pResource) {}
	virtual void Prepare() {}
	D3D12_RESOURCE_STATES GetCurrentResourceState() { return m_currentResourceState; }
	void SetCurrentResourceState(D3D12_RESOURCE_STATES state);
	ID3D12Resource* GetResource() { return m_pResources[0]; }

protected:
	GraphicsDevice*					m_pGraphicsDevice = nullptr;
	ID3D12DescriptorHeap*			m_pRTVHeap = nullptr;
	std::vector<ID3D12Resource*>	m_pResources;
	D3D12_RESOURCE_STATES m_currentResourceState;
};