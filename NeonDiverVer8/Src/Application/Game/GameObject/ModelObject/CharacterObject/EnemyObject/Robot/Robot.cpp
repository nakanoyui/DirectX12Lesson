#include "Robot.h"

#include "Application/Game/GameProcess.h"
#include "Application/Game/GameManager.h"

#include "../../PlayerObject/PlayerManager.h"
#include "../../EnemyObject/EnemyManager.h"

void Robot::LoadAsset()
{
	ReferenceJson();

	m_wpAssetManager = Application::Instance().GetGameProcess()->GetGameManager()->GetAssetManager();
	m_spModelWork = m_wpAssetManager.lock()->GetModelWork(m_wpJsonManager.lock()->GetAssetJson().at("Robot").at("Model"));

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

	m_upLHandDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upLHandDebugWireFrame->CreateLineCube(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("X").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Y").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Z").get<float>() * Constant::Two_F), RedColor);

	m_upRHandDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upRHandDebugWireFrame->CreateLineCube(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("X").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Y").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Z").get<float>() * Constant::Two_F), RedColor);

	m_upSearchDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upSearchDebugWireFrame->CreateLineSphere(m_wpJson.lock()->GetConstJson().at("SearchRange").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("DebugWireFrameVertices").get<float>(), OrangeColor);

	m_upAttackDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upAttackDebugWireFrame->CreateLineSphere(m_wpJson.lock()->GetConstJson().at("AttackRange").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("DebugWireFrameVertices").get<float>(), YellowColor);
}

void Robot::Init()
{
	EnemyObject::Init();

	m_transform.Positon(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("StartPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("StartPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("StartPos").at("Z")));

	m_bStartAttack = false;
	m_attackCoolTimeCount = Constant::Zero_I;
}

void Robot::Update()
{
	m_velocity = Math::Vector3::Zero;

	const auto& workNodes = m_spModelWork->GetNodes();

	for (auto&& nodeIdx : m_spModelWork->GetData()->GetBoneNodeIndices())
	{
		const auto& dataNode = m_spModelWork->GetData()->GetOriginalNodes()[nodeIdx];
		const auto& workNode = workNodes[nodeIdx];

		if (dataNode.m_name == m_wpJson.lock()->GetConstJson().at("LHand"))
		{
			m_mLHand = workNode.m_worldTransform;
		}

		if (dataNode.m_name == m_wpJson.lock()->GetConstJson().at("RHand"))
		{
			m_mRHand = workNode.m_worldTransform;
		}
	}

	if (m_bStartAttack)
	{
		m_attackCoolTimeCount--;
	}

	UpdateAnimation();

	UpdateSearch();

	m_mLHand = m_mLHand * m_transform.Matrix();
	m_mRHand = m_mRHand * m_transform.Matrix();

	UpdateCollision();

	UpdateMatrix();

	m_animationBlend.AdvanceTime(m_spModelWork->WorkNodes(), m_animeSpeed);
	m_spModelWork->CalcNodeMatrices();
}

void Robot::DrawSkinMesh()
{
	m_wpAssetManager.lock()->GetShader("SkinMeshShader")->DrawModel(*m_spModelWork, m_transform.Matrix());
}

void Robot::DrawEffect()
{
	Math::Vector3 offset;
	offset.y = m_wpJson.lock()->GetConstJson().at("BumpHeight").get<float>();
	m_wpAssetManager.lock()->GetShader("EffectShader")->DrawEffect(m_upDebugWireFrame->GetMesh().get(), m_transform.Matrix(), offset);

	m_wpAssetManager.lock()->GetShader("EffectShader")->DrawEffect(m_upSearchDebugWireFrame->GetMesh().get(), m_transform.Matrix());

	m_wpAssetManager.lock()->GetShader("EffectShader")->DrawEffect(m_upAttackDebugWireFrame->GetMesh().get(), m_transform.Matrix());

	Math::Vector3 scale;
	Math::Quaternion quaternion;
	Math::Vector3 pos;
	m_mLHand.Decompose(scale, quaternion, pos);

	Math::Matrix mRotate = Math::Matrix::CreateFromQuaternion(quaternion);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(pos);
	Math::Matrix mLocal = Math::Matrix::CreateTranslation(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Z")));

	m_wpAssetManager.lock()->GetShader("EffectShader")->DrawEffect(m_upLHandDebugWireFrame->GetMesh().get(), mLocal * mRotate * mTrans);

	m_mRHand.Decompose(scale, quaternion, pos);

	mRotate = Math::Matrix::CreateFromQuaternion(quaternion);
	mTrans = Math::Matrix::CreateTranslation(pos);
	mLocal = Math::Matrix::CreateTranslation(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Z")));

	m_wpAssetManager.lock()->GetShader("EffectShader")->DrawEffect(m_upLHandDebugWireFrame->GetMesh().get(), mLocal * mRotate * mTrans);
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
	case DamageType::KnockBack01:
		if (m_animeType == AnimeType::Damage1)break;
		m_upStateContext->SetNextState(std::make_unique<Damage1>(this));
		m_animeType = AnimeType::Damage1;
		m_attackCoolTimeCount = m_wpJson.lock()->GetConstJson().at("AttackCoolTime").get<int>();
		break;
	case DamageType::KnockBack02:
		if (m_animeType == AnimeType::Damage2)break;
		m_upStateContext->SetNextState(std::make_unique<Damage2>(this));
		m_animeType = AnimeType::Damage2;
		m_attackCoolTimeCount = m_wpJson.lock()->GetConstJson().at("AttackCoolTime").get<int>();
		break;
	case DamageType::BlowAway:
		if (m_animeType == AnimeType::Dead)break;
		m_upStateContext->SetNextState(std::make_unique<Dead>(this));
		m_animeType = AnimeType::Dead;
		m_attackCoolTimeCount = m_wpJson.lock()->GetConstJson().at("AttackCoolTime").get<int>();
		break;
	default:
		break;
	}
}

void Robot::UpdateSearch()
{
	if (m_animeType != AnimeType::Idle && m_animeType != AnimeType::Attack)
	{
		return;
	}

	Math::Vector3 distance = m_wpPlayerManager.lock()->GetPlayer().lock()->GetPos() - m_transform.Positon();
	distance.y = Constant::Zero_F;

	float SearchRange = m_wpJson.lock()->GetConstJson().at("SearchRange").get<float>() * m_wpJson.lock()->GetConstJson().at("SearchRange").get<float>();
	float AttackRange = m_wpJson.lock()->GetConstJson().at("AttackRange").get<float>() * m_wpJson.lock()->GetConstJson().at("AttackRange").get<float>();

	if (distance.LengthSquared() < SearchRange)
	{
		m_bSearchRange = true;
		if (distance.LengthSquared() > AttackRange && m_animeType != AnimeType::Attack)
		{
			distance.Normalize();
			m_velocity = distance;
		}
		else
		{
			if (m_animeType != AnimeType::Attack && m_attackCoolTimeCount <= Constant::Zero_I)
			{
				m_upStateContext->SetNextState(std::make_unique<Attack>(this));
				m_animeType = AnimeType::Attack;
				m_bStartAttack = true;
				m_attackCoolTimeCount = m_wpJson.lock()->GetConstJson().at("AttackCoolTime").get<int>();
				m_attackVelocity = distance;
				m_attackVelocity.Normalize();
			}
			else
			{
				if (m_animationBlend.GetAnimator()->GetNowAnimationTime() > m_wpJson.lock()->GetConstJson().at("AnimationAttackStartTime").get<float>() &&
					m_animationBlend.GetAnimator()->GetNowAnimationTime() < m_wpJson.lock()->GetConstJson().at("AnimationAttackEndTime").get<float>())
				{
					m_velocity = m_attackVelocity * m_wpJson.lock()->GetConstJson().at("AttackAttenuation").get<float>();
				}
			}
		}
	}
	else
	{
		m_bSearchRange = false;
	}
}

void Robot::UpdateCollision()
{
	DirectX::BoundingSphere myBs;
	myBs.Center = m_transform.Positon();
	myBs.Center.y += m_wpJson.lock()->GetConstJson().at("BumpHeight");
	myBs.Radius = m_wpJson.lock()->GetConstJson().at("HitToRadius");

	for (const auto& wpEnemy : Application::Instance().GetGameProcess()->GetGameManager()->GetEnemyManager()->GetEnemyObjects())
	{
		if (wpEnemy.lock().get() == this)continue;

		std::list<Collider::CollisionResult> collisionResults;

		if (wpEnemy.lock()->GetCollider()->Intersects(Collider::Type::TypeBump, myBs, &collisionResults))
		{
			for (const Collider::CollisionResult& result : collisionResults)
			{
				Math::Vector3 pushVec = result.m_hitDir * result.m_overlapDistance;

				m_transform.Positon(m_transform.Positon() + pushVec);
			}
		}
	}

	if (m_animeType != AnimeType::Attack)return;

	if (m_animationBlend.GetAnimator()->GetNowAnimationTime() < m_wpJson.lock()->GetConstJson().at("AnimationAttackStartTime").get<float>() ||
		m_animationBlend.GetAnimator()->GetNowAnimationTime() > m_wpJson.lock()->GetConstJson().at("AnimationAttackEndTime").get<float>())
	{
		return;
	}

	DirectX::BoundingSphere bs;
	bs.Center = m_wpPlayerManager.lock()->GetPlayer().lock()->GetPos();
	bs.Center.y += m_wpJson.lock()->GetConstJson().at("BumpHeight");
	bs.Radius = m_wpJson.lock()->GetConstJson().at("HitToRadius");

	Math::Vector3 scale;
	Math::Quaternion quaternion;
	Math::Vector3 pos;
	m_mLHand.Decompose(scale, quaternion, pos);

	Math::Matrix mRotate = Math::Matrix::CreateFromQuaternion(quaternion);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(pos);
	Math::Matrix mLocal = Math::Matrix::CreateTranslation(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Z")));

	DirectX::BoundingBox lHandOBB;
	lHandOBB.Transform(lHandOBB, mLocal * mRotate * mTrans);
	lHandOBB.Extents = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("X").get<float>(),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Y").get<float>(),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Z").get<float>());

	if (lHandOBB.Intersects(bs))
	{
		if (m_wpPlayerManager.lock()->GetPlayer().lock()->GetLimPower())
		{
			if (m_wpPlayerManager.lock()->GetPlayer().lock()->GetDamageType() != DamageType::KnockBack01)
			{
				m_wpPlayerManager.lock()->GetPlayer().lock()->SetJustDodge(true);
			}
		}
		else
		{
			if (m_wpPlayerManager.lock()->GetPlayer().lock()->GetDamageType() != DamageType::KnockBack01)
			{
				m_wpPlayerManager.lock()->GetPlayer().lock()->SetDamageType(DamageType::KnockBack01);
			}
		}
	}

	m_mRHand.Decompose(scale, quaternion, pos);

	mRotate = Math::Matrix::CreateFromQuaternion(quaternion);
	mTrans = Math::Matrix::CreateTranslation(pos);
	mLocal = Math::Matrix::CreateTranslation(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Z")));

	DirectX::BoundingBox rHandOBB;
	rHandOBB.Transform(rHandOBB, mLocal * mRotate * mTrans);
	rHandOBB.Extents = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("X").get<float>(),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Y").get<float>(),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Z").get<float>());

	if (rHandOBB.Intersects(bs))
	{
		if (m_wpPlayerManager.lock()->GetPlayer().lock()->GetLimPower())
		{
			if (m_wpPlayerManager.lock()->GetPlayer().lock()->GetDamageType() != DamageType::KnockBack01)
			{
				m_wpPlayerManager.lock()->GetPlayer().lock()->SetJustDodge(true);
			}
		}
		else
		{
			if (m_wpPlayerManager.lock()->GetPlayer().lock()->GetDamageType() != DamageType::KnockBack01)
			{
				m_wpPlayerManager.lock()->GetPlayer().lock()->SetDamageType(DamageType::KnockBack01);
			}
		}
	}
}

void Robot::UpdateMatrix()
{
	m_transform.Positon(m_transform.Positon() + (m_velocity * m_wpJson.lock()->GetConstJson().at("VelocityScalar").get<float>()));
	m_transform.Scale(m_wpJson.lock()->GetConstJson().at("ScaleSize").get<float>());

	if (m_bSearchRange && m_animeType != AnimeType::Dead)
	{
		Math::Vector3 nowPos = m_transform.Positon();
		Math::Vector3 targetPos = m_wpPlayerManager.lock()->GetPlayer().lock()->GetPos();
		Math::Vector3 targetVec = targetPos - nowPos;
		targetVec.y = Constant::Zero_F;
		Math::Quaternion quaternion = Math::Quaternion::LookRotation(targetVec, Math::Vector3::Up);

		m_transform.Quaternion(Math::Quaternion::Slerp(m_transform.Quaternion(), quaternion, m_wpJson.lock()->GetConstJson().at("LookBackSpeed").get<float>()));
	}

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

void Robot::ReferenceJson()
{
	m_wpJsonManager = Application::Instance().GetGameProcess()->GetGameManager()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Robot");
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
	pRobot->SetAnimeSpeed(pRobot->GetJson().lock()->GetConstJson().at("AnimationSpeedAttack").get<float>());
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