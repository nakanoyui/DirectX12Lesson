#pragma once

#include "../CBufferAllocater/CBufferStruct/CBufferStruct.h"

class Shader
{
public:
	Shader(GraphicsDevice* pGraphicsDevice, const std::wstring& fileName);
	~Shader();

	void Create(const std::vector<InputLayout>& inputLayouts, const std::vector<RangeType>& rangeTypes,
		BlendMode blendMode = BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, bool bDepth = true, bool bDepthMask = true, int rtvCount = 1, bool alphaToCovarage = false);

	bool LoadShaderFile(const std::wstring& filepath);
	bool SetInputLayout(const std::vector<InputLayout>& layouts);
	bool SetRootParameters(const std::vector<RangeType>& ranges);
	bool SetBlendMode(BlendMode mode);
	bool SetPrimitiveTopology(PrimitiveTopologyType type);
	bool SetIsUseDepth(bool depthFlag);
	bool SetIsUseDepthMask(bool bDepthMask);
	bool SetRtvCount(unsigned int value);
	bool SetAlphaToCoverageEnable(bool flag);
	void CreatePipeline();

	void Begin();

	void SetMaterial(const Material& material, const Math::Vector4& colRate);

	void DrawTex(Texture* pTexture, Math::Matrix mWorld = Math::Matrix::Identity);

	/// <summary>
	/// ���b�V����`��
	/// </summary>
	/// <param name="pMesh">���b�V��</param>
	/// <param name="mWorld">���[���h�s��</param>
	/// <param name="materials">�}�e���A�����</param>
	/// <param name="col">�F</param>
	void DrawMesh(const Mesh* pMesh, const Math::Matrix& mWorld, const std::vector<Material>& materials, const Math::Vector4& col);

	/// <summary>
	/// ���f���f�[�^��`��
	/// </summary>
	/// <param name="rModel">���f���f�[�^</param>
	/// <param name="mWorld">���[���h�s��</param>
	/// <param name="col">�F</param>
	void DrawModel(const ModelData& rModel, const Math::Matrix& mWorld = Math::Matrix::Identity, const Math::Color& col = WhiteColor);

	/// <summary>
	/// ���f�����[�N�̕`��
	/// </summary>
	/// <param name="rModel">���f�����[�N</param>
	/// <param name="mWorld">���[���h�s��</param>
	/// <param name="col">�F</param>
	void DrawModel(const ModelWork& rModel, const Math::Matrix& mWorld = Math::Matrix::Identity, const Math::Color& col = WhiteColor);

	/// <summary>
	/// �G�t�F�N�g�̕`��
	/// </summary>
	/// <param name="pMesh">���b�V��</param>
	/// <param name="mWorld">���[���h�s��</param>
	void DrawEffect(Mesh* pMesh, const Math::Matrix& mWorld);

	int GetCBVCount() { return m_cbvCount; }

private:
	GraphicsDevice* m_pGraphicsDevice = nullptr;
	Pipeline* m_pPipeline = nullptr;
	RootSignature* m_pRootSignature = nullptr;

	std::vector<InputLayout>	m_inputLayouts;
	std::vector<RangeType>		m_ranges;
	BlendMode					m_blendMode;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE	m_topologyType;
	bool			m_bDepth;
	bool			m_bDepthMask;
	unsigned int	m_rtvCount;
	bool			m_bAlphaToCoverageEnable;

	int				m_cbvCount = 0;

	static const int MAX_BONE_BUFFER = 300;
	std::array<ConstantBuffer::cbBones, MAX_BONE_BUFFER> m_cbBones;

	ID3DBlob* m_pVSBlob = nullptr;
	ID3DBlob* m_pHSBlob = nullptr;
	ID3DBlob* m_pDSBlob = nullptr;
	ID3DBlob* m_pGSBlob = nullptr;
	ID3DBlob* m_pPSBlob = nullptr;
};