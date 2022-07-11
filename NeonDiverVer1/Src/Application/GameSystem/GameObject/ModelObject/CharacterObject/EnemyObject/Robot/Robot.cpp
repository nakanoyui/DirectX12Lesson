#include "Robot.h"

void Robot::LoadAsset()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Robot");

	m_spModelWork = std::make_shared<ModelWork>();
	m_spModelWork->Load(m_wpJsonManager.lock()->GetAssetJson().at("Robot").at("Model"));
}

void Robot::Init()
{
	m_wpShaderManager = Application::Instance().GetGameSystem()->GetShaderManager();

	m_mWorld.Translation(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("StartPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("StartPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("StartPos").at("Z")));
}

void Robot::Update()
{
}

void Robot::DrawStandard()
{
	m_wpShaderManager.lock()->GetShader("StandardShader")->DrawModel(*m_spModelWork, m_mWorld);
}