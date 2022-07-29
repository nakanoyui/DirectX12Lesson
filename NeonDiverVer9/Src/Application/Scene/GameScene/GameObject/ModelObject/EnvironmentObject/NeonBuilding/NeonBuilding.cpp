#include "NeonBuilding.h"

#include "Application/Application.h"

void NeonBuilding::LoadAsset()
{
	ReferenceJson();

	m_wpAssetManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetAssetManager();
	m_spModelWork = m_wpAssetManager.lock()->GetModelWork(m_wpJsonManager.lock()->GetAssetJson().at("NeonBuilding").at("Model"));

	if (!m_upCollider)
	{
		m_upCollider = std::make_unique<Collider>(*this);
	}

	m_upCollider->AddCollisionShape(std::make_shared<ModelCollision>(m_spModelWork, Collider::Type::TypeBump));

	m_upDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upDebugWireFrame->CreateLineCube(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("X").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Y").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Z").get<float>() * Constant::Two_F), GreenColor);
}

void NeonBuilding::Init()
{
	m_transform.RotationY(m_stageData.m_rotate);
	m_transform.Positon(m_stageData.m_pos);
}

void NeonBuilding::Update()
{
	m_transform.MatrixComposition();
}

void NeonBuilding::DrawStandard()
{
	m_wpAssetManager.lock()->GetShader("StandardShader")->DrawModel(*m_spModelWork, m_transform.Matrix());
}

void NeonBuilding::DrawEffect()
{
	auto pos = m_transform.Positon();
	Math::Matrix mTrans = Math::Matrix::CreateTranslation({ pos.x,pos.y + m_wpJson.lock()->GetConstJson().at("BumpHeight"),pos.z });

	m_wpAssetManager.lock()->GetShader("EffectShader")->DrawEffect(m_upDebugWireFrame->GetMesh().get(), mTrans);
}

void NeonBuilding::ReferenceJson()
{
	m_wpJsonManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("NeonBuilding");
}