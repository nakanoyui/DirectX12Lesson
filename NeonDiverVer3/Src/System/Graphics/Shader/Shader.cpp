#include "Shader.h"

Shader::Shader(GraphicsDevice* pGraphicsDevice, const std::wstring& fileName)
	:m_pGraphicsDevice(pGraphicsDevice)
{
	LoadShaderFile(fileName);
}

Shader::~Shader()
{
	SafeRelease(m_pVSBlob);
	SafeRelease(m_pHSBlob);
	SafeRelease(m_pDSBlob);
	SafeRelease(m_pGSBlob);
	SafeRelease(m_pPSBlob);
	SafeDelete(m_pPipeline);
}

void Shader::Create(const std::vector<InputLayout>& inputLayouts, const std::vector<RangeType>& rangeTypes, BlendMode blendMode, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, bool depthFlag, int rtvCount, bool alphaToCovarage)
{
	if (!SetInputLayout(inputLayouts)) { assert(0 && "頂点インプットのセットに失敗しました"); }
	if (!SetRootParameters(rangeTypes)) { assert(0 && "ルートパラメータのセットに失敗しました"); }
	if (!SetBlendMode(blendMode)) { assert(0 && "ブレンドモードのセットに失敗しました"); }
	if (!SetPrimitiveTopology((PrimitiveTopologyType)topologyType)) { assert(0 && "プリミティブトポロジーのセットに失敗しました"); }
	if (!SetIsUseDepth(depthFlag)) { assert(0 && "デプスフラグのセットに失敗しました"); }
	if (!SetRtvCount(rtvCount)) { assert(0 && "レンダーターゲットのセットに失敗しました"); }
	if (!SetAlphaToCoverageEnable(alphaToCovarage)) { assert(0 && "アルファトゥカバレッジのセットに失敗しました"); }

	CreatePipeline();
}

bool Shader::LoadShaderFile(const std::wstring& filepath)
{
	ID3DInclude* include = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	UINT flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;

	std::wstring format = L".hlsl";
	std::wstring currentPath = L"Asset/Shader/";

	std::wstring fullFilepath = currentPath + filepath + L"_VS" + format;
	result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main", "vs_5_0", flag, 0, &m_pVSBlob, &errorBlob);

	if (FAILED(result))
	{
		assert("シェーダーのコンパイルに失敗しました");
	}

	fullFilepath = currentPath + filepath + L"_PS" + format;
	result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main", "ps_5_0", flag, 0, &m_pPSBlob, &errorBlob);

	if (FAILED(result))
	{
		assert("シェーダーのコンパイルに失敗しました");
	}

	return true;
}

bool Shader::SetInputLayout(const std::vector<InputLayout>& layouts)
{
	if ((int)layouts.size() == 0)
	{
		return false;
	}

	m_inputLayouts = layouts;
	return true;
}

bool Shader::SetRootParameters(const std::vector<RangeType>& ranges)
{
	if ((int)ranges.size() == 0)
	{
		return false;
	}

	m_ranges = ranges;
	return true;
}

bool Shader::SetBlendMode(BlendMode mode)
{
	m_blendMode = mode;

	return true;
}

bool Shader::SetPrimitiveTopology(PrimitiveTopologyType type)
{
	m_topologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)type;

	return true;
}

bool Shader::SetIsUseDepth(bool depthFlag)
{
	m_bDepthFlag = depthFlag;

	return true;
}

bool Shader::SetRtvCount(unsigned int value)
{
	m_rtvCount = value;

	return true;
}

bool Shader::SetAlphaToCoverageEnable(bool flag)
{
	m_bAlphaToCoverageEnable = flag;

	return true;
}

void Shader::CreatePipeline()
{
	m_pRootSignature = new RootSignature(m_pGraphicsDevice, m_ranges);
	m_pRootSignature->Create(m_cbvCount);

	m_pPipeline = new Pipeline(m_pGraphicsDevice, m_pRootSignature, m_inputLayouts, m_blendMode, m_topologyType);
	m_pPipeline->Create({ m_pVSBlob,m_pPSBlob,m_pGSBlob,m_pHSBlob,m_pDSBlob }, m_bDepthFlag, m_rtvCount, m_bAlphaToCoverageEnable);
}

void Shader::Begin()
{
	m_pGraphicsDevice->GetCmdList()->SetPipelineState(m_pPipeline->GetPipeline());

	//ルートシグネチャのセット
	m_pGraphicsDevice->GetCmdList()->SetGraphicsRootSignature(m_pRootSignature->Get());

	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = m_pPipeline->GetTopologyType();

	switch (topologyType)
	{
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT:
		m_pGraphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		break;
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE:
		m_pGraphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE:
		m_pGraphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH:
		m_pGraphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		break;
	}
}

void Shader::SetMaterial(const Material& material, const Math::Vector4& colRate)
{
	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach(
		3, ConstantBuffer::cbMaterial{ material.BaseColor * colRate,material.Emissive,material.Metallic,material.Roughness });

	material.BaseColorTex->Set(m_cbvCount);
	material.NormalTex->Set(m_cbvCount + 1);
	material.MetallicRoughnessTex->Set(m_cbvCount + 2);
	if (material.EmissiveTex)material.EmissiveTex->Set(m_cbvCount + 3);
}

void Shader::DrawTex(Texture* pTexture, Math::Matrix mWorld)
{
	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach(2, ConstantBuffer::cbWorld{ mWorld });

	pTexture->Set(m_cbvCount);

	pTexture->GetMesh()->DrawInstanced();
}

void Shader::DrawMesh(const Mesh* pMesh, const Math::Matrix& mWorld, const std::vector<Material>& materials, const Math::Vector4& col)
{
	if (pMesh == nullptr)return;

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach(2, ConstantBuffer::cbWorld{ mWorld });

	// 全サブセット
	for (UINT subi = 0; subi < pMesh->GetSubsets().size(); subi++)
	{
		// 面が１枚も無い場合はスキップ
		if (pMesh->GetSubsets()[subi].FaceCount == 0)continue;

		// マテリアルセット
		const Material& material = materials[pMesh->GetSubsets()[subi].MaterialNo];

		SetMaterial(material, col);

		//-----------------------
		// サブセット描画
		//-----------------------
		pMesh->DrawSubset(subi);
	}
}

void Shader::DrawModel(const ModelData& rModel, const Math::Matrix& mWorld, const Math::Color& col)
{
	auto& dataNodes = rModel.GetOriginalNodes();

	// 全メッシュノードを描画
	for (auto& nodeIdx : rModel.GetMeshNodeIndices())
	{
		auto& rDataNode = dataNodes[nodeIdx];

		// 描画
		DrawMesh(rDataNode.m_spMesh.get(), rDataNode.m_worldTransform * mWorld, rModel.GetMaterials(), col);
	}
}

void Shader::DrawModel(const ModelWork& rModel, const Math::Matrix& mWorld, const Math::Color& col)
{
	// 有効じゃないときはスキップ
	if (!rModel.IsEnable()) { return; }

	const std::shared_ptr<ModelData>& data = rModel.GetData();

	auto& workNodes = rModel.GetNodes();

	// モデルがないときはスキップ
	if (data == nullptr) { return; }

	auto& dataNodes = data->GetOriginalNodes();

	// スキンメッシュモデルの場合：ボーン情報を書き込み
	if (data->IsSkinMesh())
	{
		for (auto&& nodeIdx : data->GetBoneNodeIndices())
		{
			if (nodeIdx >= MAX_BONE_BUFFER) { assert(0 && "転送できるボーンの上限数を超えました"); return; }

			auto& dataNode = dataNodes[nodeIdx];
			auto& workNode = workNodes[nodeIdx];

			// ボーン情報からGPUに渡す行列の計算
			m_cbBones[dataNode.m_boneIndex].m_mBone = dataNode.m_boneInverseWorldMatrix * workNode.m_worldTransform;
		}
		GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttachData(4, &m_cbBones, sizeof(m_cbBones));
	}

	// 全メッシュノードを描画
	for (auto& nodeIdx : rModel.GetDrawMeshNodeIndices())
	{
		auto& rDataNode = dataNodes[nodeIdx];
		auto& rWorkNode = workNodes[nodeIdx];

		const std::shared_ptr<Mesh>& spMesh = rDataNode.m_spMesh;

		// 描画
		DrawMesh(spMesh.get(), rWorkNode.m_worldTransform * mWorld, data->GetMaterials(), col);
	}
}

void Shader::DrawEffect(Mesh* pMesh, Math::Matrix mWorld, const Math::Vector3& offset)
{
	if (pMesh == nullptr)return;

	Math::Vector3 result = mWorld.Translation() + offset;
	mWorld.Translation(result);

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach(2, ConstantBuffer::cbWorld{ mWorld });

	pMesh->DrawInstanced();
}