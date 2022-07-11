#include "Sword.h"

#include "../../CharacterObject/PlayerObject/PlayerManager.h"

void Sword::LoadAsset()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Sword");

	m_spModelData = std::make_shared<ModelData>();
	m_spModelData->Load(m_wpJsonManager.lock()->GetAssetJson().at("Sword").at("Model"));
}

void Sword::Init()
{
	m_wpPlayerManager = Application::Instance().GetGameSystem()->GetPlayerManager();
	m_wpShaderManager = Application::Instance().GetGameSystem()->GetShaderManager();
}

void Sword::Update()
{
	auto& workNodes = m_wpPlayerManager.lock()->GetPlayer().lock()->GetModelWork()->GetNodes();
	// 全ノードから右手のボーンを検索する
	for (auto&& nodeIdx : m_wpPlayerManager.lock()->GetPlayer().lock()->GetModelWork()->GetData()->GetBoneNodeIndices())
	{
		auto& dataNode = m_wpPlayerManager.lock()->GetPlayer().lock()->GetModelWork()->GetData()->GetOriginalNodes()[nodeIdx];
		auto& workNode = workNodes[nodeIdx];

		if (dataNode.m_name == m_wpJson.lock()->GetConstJson().at("HoldPlace"))
		{
			m_mWorld = workNode.m_worldTransform;
			return;
		}
	}
}

void Sword::DrawStandard()
{
	m_wpShaderManager.lock()->GetShader("StandardShader")->DrawModel(*m_spModelData, m_mWorld * m_wpPlayerManager.lock()->GetPlayer().lock()->GetMatrix());
}