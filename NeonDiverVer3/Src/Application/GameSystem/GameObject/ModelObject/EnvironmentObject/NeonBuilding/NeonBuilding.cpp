#include "NeonBuilding.h"

void NeonBuilding::LoadAsset()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("NeonBuilding");

	m_spModelData = std::make_shared<ModelData>();
	m_spModelData->Load(m_wpJsonManager.lock()->GetAssetJson().at("NeonBuilding").at("Model"));
}

void NeonBuilding::Init()
{
	m_wpShaderManager = Application::Instance().GetGameSystem()->GetShaderManager();

	m_mWorld = Math::Matrix::CreateScale(m_wpJson.lock()->GetConstJson().at("Scale")) *
		Math::Matrix::CreateTranslation(
			m_wpJson.lock()->GetConstJson().at("StartPos").at("X").get<float>(), 
			m_wpJson.lock()->GetConstJson().at("StartPos").at("Y").get<float>(),
			m_wpJson.lock()->GetConstJson().at("StartPos").at("Z").get<float>());
}

void NeonBuilding::Update()
{
}

void NeonBuilding::DrawStandard()
{
	m_wpShaderManager.lock()->GetShader("StandardShader")->DrawModel(*m_spModelData, m_mWorld);
}