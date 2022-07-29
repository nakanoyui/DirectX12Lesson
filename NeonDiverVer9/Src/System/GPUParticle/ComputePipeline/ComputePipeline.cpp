#include "ComputePipeline.h"

#include "Graphics/Shader/RootSignature/RootSignature.h"

ComputePipeline::ComputePipeline(GraphicsDevice* pGraphicsDevice, const std::wstring& filepath):
	m_pGraphicsDevice(pGraphicsDevice)
{
	// CSのコンパイル＆生成
	std::wstring FilePath = L"Asset/Shader/" + filepath + L"_CS.hlsl";
	auto result = D3DCompileFromFile(FilePath.c_str(),nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_pCSBlob, nullptr);

	if (FAILED(result))
	{
		assert(0&&"コンピュートシェーダーのコンパイルに失敗しました");
	}
}

ComputePipeline::~ComputePipeline()
{
}

void ComputePipeline::Create(const std::vector<RangeType>& range)
{
	// ルートシグネチャの生成
	m_upRootSignature = std::make_unique<RootSignature>(m_pGraphicsDevice, range);
	int cbvCount = 0;
	m_upRootSignature->Create(cbvCount);

	D3D12_COMPUTE_PIPELINE_STATE_DESC pipeDesc = {};
	pipeDesc.CS.pShaderBytecode = m_pCSBlob->GetBufferPointer();
	pipeDesc.CS.BytecodeLength = m_pCSBlob->GetBufferSize();
	pipeDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pipeDesc.NodeMask = 0;
	pipeDesc.pRootSignature = m_upRootSignature->Get();

	auto result = m_pGraphicsDevice->GetDevice()->CreateComputePipelineState(&pipeDesc,IID_PPV_ARGS(&m_pPipeline));

	if (FAILED(result))
	{
		assert(0 && "コンピュートパイプラインの作成に失敗しました");
	}
}

void ComputePipeline::Set()
{
	m_pGraphicsDevice->GetCmdList()->SetComputeRootSignature(m_upRootSignature->Get());
	m_pGraphicsDevice->GetCmdList()->SetPipelineState(m_pPipeline.Get());
}