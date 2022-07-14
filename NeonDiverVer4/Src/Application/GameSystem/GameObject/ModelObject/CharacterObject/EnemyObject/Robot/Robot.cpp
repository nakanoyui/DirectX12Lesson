#include "Robot.h"

#include "Application/main.h"
#include "Application/GameSystem/GameSystem.h"

#include "../../PlayerObject/PlayerManager.h"

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
	bs.Center = m_transform.Positon();
	bs.Center.y += m_wpJson.lock()->GetConstJson().at("BumpHeight");
	bs.Radius = m_wpJson.lock()->GetConstJson().at("HitToRadius");
	m_upCollider->AddCollisionShape(std::make_shared<SphereCollision>(bs, Collider::Type::TypeBump | Collider::Type::TypeAttack));

	m_upStateContext = std::make_unique<AnimationStateContext>(this);

	m_animationBlend.SetAnimation(m_spModelWork->GetData()->GetAnimation("Idle"));
	m_animationBlend.SetSpeed(m_wpJson.lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	m_animeType = AnimeType::Idle;

	m_upDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upDebugWireFrame->CreateLineSphere(m_wpJson.lock()->GetConstJson().at("HitToRadius").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("DebugWireFrameVertices").get<float>(), RedColor);
}

void Robot::Init()
{
	EnemyObject::Init();

	m_wpShaderManager = Application::Instance().GetGameSystem()->GetShaderManager();

	m_transform.Positon(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("StartPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("StartPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("StartPos").at("Z")));
}

void Robot::Update()
{
	UpdateAnimation();

	UpdateCollision();

	UpdateMatrix();

	m_animationBlend.AdvanceTime(m_spModelWork->WorkNodes(), m_animeSpeed);
	m_spModelWork->CalcNodeMatrices();
}

void Robot::DrawSkinMesh()
{
	m_wpShaderManager.lock()->GetShader("SkinMeshShader")->DrawModel(*m_spModelWork, m_transform.Matrix());
}

void Robot::DrawEffect()
{
	Math::Vector3 offset;
	offset.y = m_wpJson.lock()->GetConstJson().at("BumpHeight");
	m_wpShaderManager.lock()->GetShader("EffectShader")->DrawEffect(m_upDebugWireFrame->GetMesh().get(), m_transform.Matrix(), offset);
}

void Robot::UpdateAnimation()
{
	if (m_animeType == AnimeType::Dead)return;

	if (m_animationBlend.GetAnimator()->IsAnimationEnd())
	{
		if (m_animeType != AnimeType::Idle)
		{
			m_upStateContext->SetNextState(std::make_unique<Idle>(this));
			m_animeType = AnimeType::Idle;
		}
	}

	switch (m_damageType)
	{
	case DamageType::KnockBack1:
		if (m_animeType == AnimeType::Damage1)break;
		m_upStateContext->SetNextState(std::make_unique<Damage1>(this));
		m_animeType = AnimeType::Damage1;
		break;
	case DamageType::KnockBack2:
		if (m_animeType == AnimeType::Damage2)break;
		m_upStateContext->SetNextState(std::make_unique<Damage2>(this));
		m_animeType = AnimeType::Damage2;
		break;
	case DamageType::BlowAway:
		if (m_animeType == AnimeType::Dead)break;
		m_upStateContext->SetNextState(std::make_unique<Dead>(this));
		m_animeType = AnimeType::Dead;
		break;
	default:
		break;
	}
}

void Robot::UpdateCollision()
{

}

void Robot::UpdateRotate()
{
	//Math::Vector3 nowDir = m_mWorld.Backward();
	//Math::Vector3 targetDir = m_wpPlayerManager.lock()->GetPlayer().lock()->GetPos();

	//nowDir.Normalize();
	//targetDir.Normalize();

	//float nowAng = atan2(nowDir.x, nowDir.z);
	//nowAng = DirectX::XMConvertToDegrees(nowAng);

	//float targetAng = atan2(targetDir.x, targetDir.z);
	//targetAng = DirectX::XMConvertToDegrees(targetAng);

	//float betweenAng = targetAng - nowAng;

	//if (betweenAng > DirectX::XMConvertToDegrees(static_cast<float>(M_PI)))
	//{
	//	betweenAng -= DirectX::XMConvertToDegrees(static_cast<float>(M_PI) * Constant::Two_F);
	//}
	//else if (betweenAng < -DirectX::XMConvertToDegrees(static_cast<float>(M_PI)))
	//{
	//	betweenAng += DirectX::XMConvertToDegrees(static_cast<float>(M_PI) * Constant::Two_F);
	//}

	//float rotAng = std::clamp(betweenAng, -m_wpJson.lock()->GetConstJson().at("LookBackSpeed").get<float>(), m_wpJson.lock()->GetConstJson().at("LookBackSpeed").get<float>());

	//m_yaw += rotAng;
}

void Robot::UpdateMatrix()
{
	m_transform.Scale(m_wpJson.lock()->GetConstJson().at("ScaleSize").get<float>());

	Math::Matrix mRotate;

	UpdateRotate();

	Math::Vector3 nowPos = m_transform.Positon();
	Math::Vector3 targetPos = m_wpPlayerManager.lock()->GetPlayer().lock()->GetPos();
	Math::Vector3 targetVec = targetPos - nowPos;
	Math::Quaternion quaternion = Math::Quaternion::LookRotation(targetVec, Math::Vector3::Up);

	m_transform.Quaternion(Math::Quaternion::Slerp(m_transform.Quaternion(), quaternion, m_wpJson.lock()->GetConstJson().at("LookBackSpeed").get<float>()));

	m_transform.MatrixComposition();
}

void Robot::ImGuiUpdate()
{
}

void Robot::Reset()
{
	m_upStateContext->SetNextState(std::make_unique<Idle>(this));
	m_animeType = AnimeType::Idle;
}

Robot::Idle::Idle(Robot* pRobot)
{
	pRobot->GetAnimationBlend().SetAnimation(pRobot->GetModelWork()->GetData()->GetAnimation("Idle"));
	pRobot->GetAnimationBlend().SetSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pRobot->SetAnimeSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Robot::Attack::Attack(Robot* pRobot)
{
	pRobot->GetAnimationBlend().SetAnimation(pRobot->GetModelWork()->GetData()->GetAnimation("Attack"), false);
	pRobot->GetAnimationBlend().SetSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pRobot->SetAnimeSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Robot::Damage1::Damage1(Robot* pRobot)
{
	pRobot->GetAnimationBlend().SetAnimation(pRobot->GetModelWork()->GetData()->GetAnimation("Damage1"), false);
	pRobot->GetAnimationBlend().SetSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pRobot->SetAnimeSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationSpeedDamage").get<float>());
}

Robot::Damage2::Damage2(Robot* pRobot)
{
	pRobot->GetAnimationBlend().SetAnimation(pRobot->GetModelWork()->GetData()->GetAnimation("Damage2"), false);
	pRobot->GetAnimationBlend().SetSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pRobot->SetAnimeSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationSpeedDamage").get<float>());
}

Robot::Dead::Dead(Robot* pRobot)
{
	pRobot->GetAnimationBlend().SetAnimation(pRobot->GetModelWork()->GetData()->GetAnimation("Dead"), false);
	pRobot->GetAnimationBlend().SetSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pRobot->SetAnimeSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationSpeedDead").get<float>());
}