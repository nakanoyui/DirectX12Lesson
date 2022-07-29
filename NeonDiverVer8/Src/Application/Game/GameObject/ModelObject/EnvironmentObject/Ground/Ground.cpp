#include "Ground.h"

#include "Application/Game/GameProcess.h"
#include "Application/Game/GameManager.h"

void Ground::LoadAsset()
{
	ReferenceJson();

	m_wpAssetManager = Application::Instance().GetGameProcess()->GetGameManager()->GetAssetManager();
	m_spModelWork = m_wpAssetManager.lock()->GetModelWork(m_wpJsonManager.lock()->GetAssetJson().at("Ground").at("Model"));

	if (!m_upCollider)
	{
		m_upCollider = std::make_unique<Collider>(*this);
	}

	m_upCollider->AddCollisionShape(std::make_shared<ModelCollision>(m_spModelWork, Collider::Type::TypeGround));
}

void Ground::Init()
{
	m_transform.RotationY(m_stageData.m_rotate);
	m_transform.Positon(m_stageData.m_pos);
}

void Ground::Update()
{
	m_transform.MatrixComposition();
}

void Ground::DrawStandard()
{
	m_wpAssetManager.lock()->GetShader("StandardShader")->DrawModel(*m_spModelWork, m_transform.Matrix());
}

void Ground::ReferenceJson()
{
	m_wpJsonManager = Application::Instance().GetGameProcess()->GetGameManager()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Ground");
}