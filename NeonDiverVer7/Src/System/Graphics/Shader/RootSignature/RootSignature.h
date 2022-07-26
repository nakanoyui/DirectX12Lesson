#pragma once

enum class RangeType
{
	CBV,
	SRV,
	UAV,
};

enum class TextureAddressMode
{
	WRAP,
	CLAMP,
};

enum class D3D12Filter
{
	POINT,
	LINEAR,
};

class RootSignature
{
public:
	RootSignature(GraphicsDevice* pGraphicsDevice, const std::vector<RangeType>& rangeTypes);
	~RootSignature();

	void Create(int& cbvCount);
	ID3D12RootSignature* Get() { return m_pRootSignature; }

private:
	void CreateRange(D3D12_DESCRIPTOR_RANGE* pRange, RangeType type, int count);
	void CreateSampler(D3D12_STATIC_SAMPLER_DESC& samplerDesc);
	void CreateStaticSampler(D3D12_STATIC_SAMPLER_DESC* pSamplerDesc, TextureAddressMode mode, D3D12Filter filter, int count);

	GraphicsDevice* m_pGraphicsDevice = nullptr;
	std::vector<RangeType>	m_rangeTypes;
	ID3DBlob* m_pRootBlob = nullptr;
	ID3D12RootSignature* m_pRootSignature = nullptr;
};