#include "RootSignature.h"

RootSignature::RootSignature(GraphicsDevice* pGraphicsDevice, const std::vector<RangeType>& rangeTypes)
	:m_pGraphicsDevice(pGraphicsDevice)
	, m_rangeTypes(rangeTypes)
{
}

RootSignature::~RootSignature()
{
	SafeRelease(m_pRootSignature);
	SafeRelease(m_pRootBlob);
}

void RootSignature::Create(int &cbvCount)
{
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	int rangeCount = (int)m_rangeTypes.size();
	// コンストラクタで指定されたレンジ数だけルートパラメータ、レンジを生成
	D3D12_ROOT_PARAMETER* pRootParam = new D3D12_ROOT_PARAMETER[rangeCount];
	D3D12_DESCRIPTOR_RANGE* pRange = new D3D12_DESCRIPTOR_RANGE[rangeCount];
	// SRVの数だけSamplerDescを生成
	int samplerCount = 0;
	for (int i = 0; i < (int)m_rangeTypes.size(); ++i)
	{
		if (m_rangeTypes[i] == RangeType::SRV)
		{
			++samplerCount;
		}
	}
	D3D12_STATIC_SAMPLER_DESC* samplerDesc = new D3D12_STATIC_SAMPLER_DESC[samplerCount];
	// 指定された順に設定
	samplerCount = 0;
	bool bSampler = false;
	int uavCount = 0;

	for (int i = 0; i < rangeCount; ++i)
	{
		switch (m_rangeTypes[i])
		{
		case RangeType::CBV:
			CreateRange(&pRange[i], RangeType::CBV, cbvCount);
			pRootParam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			pRootParam[i].DescriptorTable.pDescriptorRanges = &pRange[i];
			pRootParam[i].DescriptorTable.NumDescriptorRanges = 1;
			pRootParam[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			++cbvCount;
			break;
		case RangeType::SRV:
			CreateRange(&pRange[i], RangeType::SRV, samplerCount);
			pRootParam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			pRootParam[i].DescriptorTable.pDescriptorRanges = &pRange[i];
			pRootParam[i].DescriptorTable.NumDescriptorRanges = 1;
			pRootParam[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			++samplerCount;
			bSampler = true;
			break;
		case RangeType::UAV:
			CreateRange(&pRange[i], RangeType::UAV, uavCount);
			pRootParam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			pRootParam[i].DescriptorTable.pDescriptorRanges = &pRange[i];
			pRootParam[i].DescriptorTable.NumDescriptorRanges = 1;
			pRootParam[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			++uavCount;
			break;
		default:
			break;
		}
	}

	D3D12_STATIC_SAMPLER_DESC* pStaticSamplerDesc = new D3D12_STATIC_SAMPLER_DESC[4];
	if (bSampler)
	{
		CreateStaticSampler(&pStaticSamplerDesc[0], TextureAddressMode::WRAP, D3D12Filter::POINT, 0);
		CreateStaticSampler(&pStaticSamplerDesc[1], TextureAddressMode::CLAMP, D3D12Filter::POINT, 1);
		CreateStaticSampler(&pStaticSamplerDesc[2], TextureAddressMode::WRAP, D3D12Filter::LINEAR, 2);
		CreateStaticSampler(&pStaticSamplerDesc[3], TextureAddressMode::CLAMP, D3D12Filter::LINEAR, 3);
	}
	rootSignatureDesc.pStaticSamplers = bSampler ? pStaticSamplerDesc : nullptr;
	rootSignatureDesc.NumStaticSamplers = bSampler ? 4 : 0;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = pRootParam;
	rootSignatureDesc.NumParameters = (int)m_rangeTypes.size();

	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &m_pRootBlob, &errorBlob);

	if (FAILED(result))
	{
		assert(0 && "ルートシグネチャ作成失敗");
	}

	result = m_pGraphicsDevice->GetDevice()->CreateRootSignature(0, m_pRootBlob->GetBufferPointer(), m_pRootBlob->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));

	if (FAILED(result))
	{
		assert(0 && "ルートシグネチャ作成失敗");
	}

	delete[] pRootParam;
	delete[] pRange;
	delete[] samplerDesc;
	delete[] pStaticSamplerDesc;
}

void RootSignature::CreateRange(D3D12_DESCRIPTOR_RANGE* pRange, RangeType type, int count)
{
	switch (type)
	{
	case RangeType::CBV:
		(*pRange) = {};
		(*pRange).NumDescriptors = 1;
		(*pRange).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		(*pRange).BaseShaderRegister = count;
		(*pRange).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		break;
	case RangeType::SRV:
		(*pRange) = {};
		(*pRange).NumDescriptors = 1;
		(*pRange).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		(*pRange).BaseShaderRegister = count;
		(*pRange).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		break;
	case RangeType::UAV:
		(*pRange) = {};
		(*pRange).NumDescriptors = 1;
		(*pRange).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		(*pRange).BaseShaderRegister = count;
		(*pRange).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		break;
	default:
		break;
	}
}

void RootSignature::CreateSampler(D3D12_STATIC_SAMPLER_DESC& samplerDesc)
{
	samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplerDesc.MaxAnisotropy = 16;
}

void RootSignature::CreateStaticSampler(D3D12_STATIC_SAMPLER_DESC* pSamplerDesc, TextureAddressMode mode, D3D12Filter filter, int count)
{
	D3D12_TEXTURE_ADDRESS_MODE addressMode = mode == TextureAddressMode::WRAP ? D3D12_TEXTURE_ADDRESS_MODE_WRAP : D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	D3D12_FILTER samplingFilter = filter == D3D12Filter::POINT ? D3D12_FILTER_MIN_MAG_MIP_POINT : D3D12_FILTER_MIN_MAG_MIP_LINEAR;

	(*pSamplerDesc) = {};
	(*pSamplerDesc).AddressU = addressMode;
	(*pSamplerDesc).AddressV = addressMode;
	(*pSamplerDesc).AddressW = addressMode;
	(*pSamplerDesc).BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	(*pSamplerDesc).Filter = samplingFilter;
	(*pSamplerDesc).MaxLOD = D3D12_FLOAT32_MAX;
	(*pSamplerDesc).MinLOD = 0.0f;
	(*pSamplerDesc).ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	(*pSamplerDesc).ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	(*pSamplerDesc).MaxAnisotropy = 16;
	(*pSamplerDesc).ShaderRegister = count;
}