#include "Ground.h"

#include "Application/main.h"
#include "Application/GameSystem/GameSystem.h"

void Ground::LoadAsset()
{
	ReferenceJson();

	m_wpModelManager = Application::Instance().GetGameSystem()->GetModelManager();
	m_wpModelManager.lock()->ResisterModelWork(m_wpJsonManager.lock()->GetAssetJson().at("Ground").at("Model"));
	m_spModelWork = m_wpModelManager.lock()->GetModelWork("Ground.gltf");

	if (!m_upCollider)
	{
		m_upCollider = std::make_unique<Collider>(*this);
	}

	m_upCollider->AddCollisionShape(std::make_shared<ModelCollision>(m_spModelWork, Collider::Type::TypeGround));
}

void Ground::Init()
{
	m_wpShaderManager = Application::Instance().GetGameSystem()->GetShaderManager();

	m_transform.RotationY(m_stageData.m_rotate);
	m_transform.Positon(m_stageData.m_pos);
}

void Ground::Update()
{
	m_transform.MatrixComposition();
}

void Ground::DrawStandard()
{
	m_wpShaderManager.lock()->GetShader("StandardShader")->DrawModel(*m_spModelWork, m_transform.Matrix());
}

void Ground::ReferenceJson()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Ground");
}