#include "Pipeline.h"

Pipeline::Pipeline(GraphicsDevice* pGraphicsDevice, RootSignature* pRootSignature, const std::vector<InputLayout>& inputLayouts, BlendMode blendMode, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType)
	:m_pGraphicsDevice(pGraphicsDevice)
	, m_upRootSignature(pRootSignature)
	, m_pInputLayouts(inputLayouts)
	, m_blendMode(blendMode)
	, m_topologyType(topologyType)
	, m_pPipelineState(nullptr)
{
}

Pipeline::~Pipeline()
{
	SafeRelease(m_pPipelineState);
}

void Pipeline::Create(std::vector<ID3DBlob*> pBlobs, bool depthFlag, int rtvCount, bool alphaCoverage)
{
	D3D12_INPUT_ELEMENT_DESC* inputLayout = new D3D12_INPUT_ELEMENT_DESC[(int)m_pInputLayouts.size()];

	for (int i = 0; i < (int)m_pInputLayouts.size(); ++i)
	{
		if (m_pInputLayouts[i] == InputLayout::POSITION)
		{
			inputLayout[i] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
		}
		else if (m_pInputLayouts[i] == InputLayout::TEXCOORD)
		{
			inputLayout[i] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,0 };
		}
		else if (m_pInputLayouts[i] == InputLayout::NORMAL)
		{
			inputLayout[i] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}
		else if (m_pInputLayouts[i] == InputLayout::COLOR)
		{
			inputLayout[i] = { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}
		else if (m_pInputLayouts[i] == InputLayout::TANGENT)
		{
			inputLayout[i] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}
		else if (m_pInputLayouts[i] == InputLayout::SKININDEX)
		{
			inputLayout[i] = { "SKININDEX", 0, DXGI_FORMAT_R16G16B16A16_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}	
		else if (m_pInputLayouts[i] == InputLayout::SKINWEIGHT)
		{
			inputLayout[i] = { "SKINWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}
	}

	// ブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	SetBlendMode(&blendDesc, m_blendMode);

	// PS0の各種設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	gpipeline.VS.pShaderBytecode = pBlobs[0]->GetBufferPointer();
	gpipeline.VS.BytecodeLength = pBlobs[0]->GetBufferSize();
	gpipeline.PS.pShaderBytecode = pBlobs[1]->GetBufferPointer();
	gpipeline.PS.BytecodeLength = pBlobs[1]->GetBufferSize();

	if (pBlobs[2])
	{
		gpipeline.GS.pShaderBytecode = pBlobs[2]->GetBufferPointer();
		gpipeline.GS.BytecodeLength = pBlobs[2]->GetBufferSize();
	}
	if (pBlobs[3])
	{
		gpipeline.HS.pShaderBytecode = pBlobs[3]->GetBufferPointer();
		gpipeline.HS.BytecodeLength = pBlobs[3]->GetBufferSize();
	}
	if (pBlobs[4])
	{
		gpipeline.DS.pShaderBytecode = pBlobs[4]->GetBufferPointer();
		gpipeline.DS.BytecodeLength = pBlobs[4]->GetBufferSize();
	}
	
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		// カリングしない
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;		// 中身を塗りつぶす

	if (depthFlag)
	{
		gpipeline.RasterizerState.DepthClipEnable = true;
		gpipeline.DepthStencilState.DepthEnable = true;
		gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}
	else
	{
		gpipeline.DepthStencilState.DepthEnable = false;
		gpipeline.DepthStencilState.StencilEnable = false;
	}

	gpipeline.BlendState.AlphaToCoverageEnable = alphaCoverage;

	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	gpipeline.BlendState.RenderTarget[0] = blendDesc;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;				// レイアウト先頭アドレス
	gpipeline.InputLayout.NumElements = (int)m_pInputLayouts.size();	// レイアウト配列の要素数

	gpipeline.PrimitiveTopologyType = (pBlobs[3] && pBlobs[4]) ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH : m_topologyType;
	gpipeline.NumRenderTargets = rtvCount;

	for (int i = 0; i < rtvCount; ++i)
	{
		gpipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;		// 0～1に正規化されたRGBA
		//gpipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 0～1に正規化されたRGBA(ガンマ補正)
	}

	gpipeline.SampleDesc.Count = 1;		// サンプリングは1ピクセルにつき1
	gpipeline.pRootSignature = m_upRootSignature->Get();
	auto result = m_pGraphicsDevice->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_pPipelineState));

	if (FAILED(result))
	{
		assert(0&&"パイプラインステートの作成に失敗しました");
	}

	delete[] inputLayout;
}

void Pipeline::SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC* pBlendDesc, BlendMode blendMode)
{
	(*pBlendDesc).RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	(*pBlendDesc).BlendEnable = true;
	(*pBlendDesc).BlendOpAlpha = D3D12_BLEND_OP_ADD;
	(*pBlendDesc).SrcBlendAlpha = D3D12_BLEND_ONE;
	(*pBlendDesc).DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (blendMode)
	{
	case BlendMode::BLENDMODE_ADD:
		// 加算合成
		(*pBlendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*pBlendDesc).SrcBlend = D3D12_BLEND_ONE;
		(*pBlendDesc).DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::BLENDMODE_SUB:
		// 減算合成
		(*pBlendDesc).BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		(*pBlendDesc).SrcBlend = D3D12_BLEND_ONE;
		(*pBlendDesc).DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::BLENDMODE_INV:
		// 色反転
		(*pBlendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*pBlendDesc).SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		(*pBlendDesc).DestBlend = D3D12_BLEND_ZERO;
		break;
	case BlendMode::BLENDMODE_ALPHA:
		// 半透明
		(*pBlendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*pBlendDesc).SrcBlend = D3D12_BLEND_SRC_ALPHA;
		(*pBlendDesc).DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	default:
		break;
	}
}