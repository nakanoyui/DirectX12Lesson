#pragma once

enum class BlendMode
{
	BLENDMODE_ADD,
	BLENDMODE_SUB,
	BLENDMODE_INV,
	BLENDMODE_ALPHA,
};

enum class InputLayout
{
	POSITION,
	TEXCOORD,
	NORMAL,
	TANGENT,
	COLOR,
	SKININDEX,
	SKINWEIGHT
};

enum class PrimitiveTopologyType
{
	UNDIFINED = 0,
	POINT = 1,
	LINE = 2,
	TRIANGLE = 3,
	PATCH = 4,
};

class RootSignature;

class Pipeline
{
public:
	Pipeline(GraphicsDevice* pGraphicsDevice, RootSignature* pRootSignature, const std::vector<InputLayout>& inputLayouts,
		BlendMode blendMode, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType);
	~Pipeline();

	void Create(std::vector<ID3DBlob*> pBlobs, bool depthFlag, int rtvCount, bool alphaCoverage);

	ID3D12PipelineState* GetPipeline() { return m_pPipelineState; }
	D3D12_PRIMITIVE_TOPOLOGY_TYPE GetTopologyType() { return m_topologyType; }

private:
	void SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC* pBlendDesc, BlendMode blendMode);

	GraphicsDevice* m_pGraphicsDevice;
	std::unique_ptr<RootSignature>	m_upRootSignature;
	std::vector<InputLayout>	m_pInputLayouts;

	BlendMode		m_blendMode;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE		m_topologyType;

	ID3D12PipelineState* m_pPipelineState = nullptr;
};