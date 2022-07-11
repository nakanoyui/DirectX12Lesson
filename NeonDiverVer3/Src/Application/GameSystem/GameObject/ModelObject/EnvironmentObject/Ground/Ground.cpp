#include "Ground.h"

void Ground::LoadAsset()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Ground");

	m_spModelData = std::make_shared<ModelData>();
	m_spModelData->Load(m_wpJsonManager.lock()->GetAssetJson().at("Ground").at("Model"));
}

void Ground::Init()
{
	m_wpShaderManager = Application::Instance().GetGameSystem()->GetShaderManager();
}

void Ground::Update()
{
}

void Ground::DrawStandard()
{
	m_wpShaderManager.lock()->GetShader("StandardShader")->DrawModel(*m_spModelData, m_mWorld);
}