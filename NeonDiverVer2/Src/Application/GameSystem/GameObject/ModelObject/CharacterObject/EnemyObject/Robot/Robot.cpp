#include "Robot.h"

void Robot::LoadAsset()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Robot");

	m_spModelWork = std::make_shared<ModelWork>();
	m_spModelWork->Load(m_wpJsonManager.lock()->GetAssetJson().at("Robot").at("Model"));

	if (!m_upCollider)
	{
		m_upCollider = std::make_unique<Collider>(*this);
	}
	DirectX::BoundingSphere bs;
	bs.Center = m_mWorld.Translation();
	bs.Center.y += m_wpJson.lock()->GetConstJson().at("BumpHeight");
	bs.Radius = m_wpJson.lock()->GetConstJson().at("HitToRadius");
	m_upCollider->AddCollisionShape(std::make_shared<SphereCollision>(bs, Collider::Type::TypeBump));

	m_upDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upDebugWireFrame->CreateLineSphere(m_wpJson.lock()->GetConstJson().at("HitToRadius").get<float>(), 50, RedColor);
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

void Robot::DrawEffect()
{
	Math::Vector3 offset;
	offset.y = m_wpJson.lock()->GetConstJson().at("BumpHeight");
	m_wpShaderManager.lock()->GetShader("EffectShader")->DrawEffect(m_upDebugWireFrame->GetMesh().get(), m_mWorld, offset);
}
