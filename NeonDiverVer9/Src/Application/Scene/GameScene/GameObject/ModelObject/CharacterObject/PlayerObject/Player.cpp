#include "Player.h"

#include "Application/Application.h"

#include "../EnemyObject/EnemyManager.h"
#include "../../EnvironmentObject/EnvironmentManager.h"

void Player::LoadAsset()
{
	ReferenceJson();

	m_wpAssetManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetAssetManager();
	m_spModelWork = m_wpAssetManager.lock()->GetModelWork(m_wpJsonManager.lock()->GetAssetJson().at("Player").at("Model"));

	if (!m_upCollider)
	{
		m_upCollider = std::make_unique<Collider>(*this);
	}

	DirectX::BoundingSphere bs;
	bs.Center = m_transform.Positon();
	bs.Center.y += m_wpJson.lock()->GetConstJson().at("BumpHeight");
	bs.Radius = m_wpJson.lock()->GetConstJson().at("HitToRadius");
	m_upCollider->AddCollisionShape(std::make_shared<SphereCollision>(bs, Collider::Type::TypeAttack));
}

void Player::Init()
{
	m_wpCameraManager = Application::Instance().GetCameraManager();
	m_wpKeyManager = Application::Instance().GetKeyManager();

	CameraInfo camInfo;
	camInfo.localPos = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("CameraLocalPos").at("X").get<float>(),
		m_wpJson.lock()->GetConstJson().at("CameraLocalPos").at("Y").get<float>(),
		m_wpJson.lock()->GetConstJson().at("CameraLocalPos").at("Z").get<float>());

	m_wpCameraManager.lock()->CreateCamera(camInfo, "PlayerCamera");
	m_wpCamera = m_wpCameraManager.lock()->GetCamera("PlayerCamera");

	m_speed = Constant::Zero_F;
	m_jumpPower = Constant::Zero_F;
	m_bJump = false;
	m_yaw = Constant::Zero_I;
	m_bJumpLanded = false;
	m_bJumpAttack = false;

	m_bJustDodge = false;
	m_limPower = Constant::Zero_F;
	m_dissolveRate = Constant::Zero_F;
	m_justDodgeSpeedCount = Constant::Zero_F;

	m_hitStopCount = Constant::Zero_I;
	m_bHitStop = false;
	m_saveAnimeType = AnimeType::Idle;

	m_upStateContext = std::make_unique<AnimationStateContext>(this);

	m_animeType = AnimeType::Idle;
	m_animationBlend.SetAnimation(m_spModelWork->GetData()->GetAnimation("Idle"));
	m_animationBlend.SetSpeed(m_wpJson.lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	m_animeSpeed = m_wpJson.lock()->GetConstJson().at("AnimationSpeed").get<float>();

	m_upDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upDebugWireFrame->CreateLineSphere(m_wpJson.lock()->GetConstJson().at("HitToRadius").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("DebugWireFrameVertices").get<float>(), BlueColor);
}

void Player::Update()
{
	CharacterObject::Update();
	m_velocity = Math::Vector3::Zero;

	if (m_wpKeyManager.lock()->GetKeyState('W', true))
	{
		if (m_wpKeyManager.lock()->GetKeyState(VK_SHIFT, true))
		{
			m_velocity.z = Constant::One_F;
		}
		else
		{
			m_velocity.z = Constant::Two_F;
		}
	}

	if (m_wpKeyManager.lock()->GetKeyState('S', true))
	{
		if (m_wpKeyManager.lock()->GetKeyState(VK_SHIFT, true))
		{
			m_velocity.z = -Constant::One_F;
		}
		else
		{
			m_velocity.z = -Constant::Two_F;
		}
	}

	if (m_wpKeyManager.lock()->GetKeyState('D', true))
	{
		if (m_wpKeyManager.lock()->GetKeyState(VK_SHIFT, true))
		{
			m_velocity.x = Constant::One_F;
		}
		else
		{
			m_velocity.x = Constant::Two_F;
		}
	}

	if (m_wpKeyManager.lock()->GetKeyState('A', true))
	{
		if (m_wpKeyManager.lock()->GetKeyState(VK_SHIFT, true))
		{
			m_velocity.x = -Constant::One_F;
		}
		else
		{
			m_velocity.x = -Constant::Two_F;
		}
	}

	if (m_wpKeyManager.lock()->GetKeyState(VK_SPACE) && !m_bJump)
	{
		m_jumpPower = m_wpJson.lock()->GetConstJson().at("JumpPower").get<float>();
		m_bJump = true;
	}

	if (m_bJump)
	{
		m_verticalVector += m_jumpPower;
		if (m_jumpPower > Constant::Zero_F)
		{
			m_jumpPower -= m_wpJson.lock()->GetConstJson().at("JumpPowerDecrement").get<float>();
		}
	}

	UpdateAnimation();

	UpdateAnimationDodge();

	m_velocity.y += m_verticalVector;

	if (!m_wpCamera.expired())
	{
		m_velocity = Math::Vector3::TransformNormal(m_velocity, m_wpCamera.lock()->GetRotationYMatrix());
	}

	if (m_velocity.LengthSquared())
	{
		if (m_speed < m_wpJson.lock()->GetConstJson().at("RunUpSpeedMax").get<float>())
		{
			m_speed += m_wpJson.lock()->GetConstJson().at("RunUpSpeed").get<float>();
		}
		else
		{
			m_speed = m_wpJson.lock()->GetConstJson().at("RunUpSpeedMax").get<float>();
		}
	}
	else
	{
		m_speed = m_wpJson.lock()->GetConstJson().at("RunUpSpeedMin").get<float>();
	}

	if (m_bJump)
	{
		m_velocity.x *= m_speed * m_wpJson.lock()->GetConstJson().at("JumpAttenuation").get<float>();
		m_velocity.z *= m_speed * m_wpJson.lock()->GetConstJson().at("JumpAttenuation").get<float>();
	}
	else
	{
		m_velocity.x *= m_speed;
		m_velocity.z *= m_speed;
	}

	UpdateCollision();

	UpdateMatrix();

	m_animationBlend.AdvanceTime(m_spModelWork->WorkNodes(), m_animeSpeed);
	m_spModelWork->CalcNodeMatrices();

	UpdateCamera();
}

void Player::DrawPlayer()
{
	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach(5, ConstantBuffer::cbLim{ m_limPower, { 0,0.5,1 },m_dissolveRate });

	m_wpAssetManager.lock()->GetTexture(m_wpJsonManager.lock()->GetAssetJson().at("Texture").at("Dissolve"))->Set(m_wpAssetManager.lock()->GetShader("PlayerShader")->GetCBVCount() + 4);
	m_wpAssetManager.lock()->GetShader("PlayerShader")->DrawModel(*m_spModelWork, m_transform.Matrix());
}

void Player::DrawEffect()
{
	Math::Vector3 offset;
	offset.y = m_wpJson.lock()->GetConstJson().at("BumpHeight");
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_transform.Positon());
	m_wpAssetManager.lock()->GetShader("EffectShader")->DrawEffect(m_upDebugWireFrame->GetMesh().get(), mTrans, offset);
}

void Player::UpdateAnimation()
{
	if (m_limPower > Constant::Zero_F)
	{
		m_limPower -= m_wpJson.lock()->GetConstJson().at("LimAttenuation").get<float>();
		return;
	}
	else
	{
		m_limPower = Constant::Zero_F;
	}

	if (m_damageType == DamageType::KnockBack01)
	{
		if (m_animeType != AnimeType::KnockBack01)
		{
			m_upStateContext->SetNextState(std::make_unique<KnockBack01>(this));
			m_animeType = AnimeType::KnockBack01;
		}

		if (m_velocity.LengthSquared())
		{
			if (m_animeType != AnimeType::Rise01 && m_animationBlend.GetAnimator()->IsAnimationEnd())
			{
				m_upStateContext->SetNextState(std::make_unique<Rise01>(this));
				m_animeType = AnimeType::Rise01;
				m_damageType = DamageType::None;
			}
		}
		m_velocity = Math::Vector3::Zero;
		m_bJumpLanded = true;
		return;
	}

	if (m_animeType == AnimeType::Rise01)
	{
		if (m_animationBlend.GetAnimator()->IsAnimationEnd())
		{
			m_upStateContext->SetNextState(std::make_unique<Idle>(this));
			m_animeType = AnimeType::Idle;
		}
		m_velocity = Math::Vector3::Zero;
		m_bJumpLanded = true;
		return;
	}

	if (m_bHitStop && m_saveAnimeType != m_animeType)
	{
		m_animationBlend.SetNowAnimeSpeed(m_wpJson.lock()->GetConstJson().at("AnimationSpeedHitStop").get<float>());
		m_hitStopCount++;
		if (m_hitStopCount > m_wpJson.lock()->GetConstJson().at("HitStopRenge").get<float>())
		{
			m_saveAnimeType = m_animeType;
		}
	}
	else
	{
		if (m_animeType == AnimeType::Combo01 || m_animeType == AnimeType::Combo02 || m_animeType == AnimeType::Combo03)
		{
			m_animationBlend.SetNowAnimeSpeed(m_wpJson.lock()->GetConstJson().at("AnimationSpeedAttack").get<float>());
			m_hitStopCount = Constant::Zero_I;
			m_bHitStop = false;
		}
	}

	if (!UpdateAnimationAttack())return;

	if (m_velocity.LengthSquared() >= Constant::Two_F && !m_wpKeyManager.lock()->GetKeyState(VK_SHIFT, true) && !m_bJump)
	{
		if (m_animeType != AnimeType::RunFast)
		{
			m_upStateContext->SetNextState(std::make_unique<RunFast>(this));
			m_animeType = AnimeType::RunFast;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}

		if (m_velocity.LengthSquared() > Constant::Two_F * Constant::Two_F)
		{
			m_velocity *= m_wpJson.lock()->GetConstJson().at("LRRunFastAttenuation").get<float>();
		}
		else
		{
			m_velocity *= m_wpJson.lock()->GetConstJson().at("RunFastAttenuation").get<float>();
		}
		m_bJumpLanded = true;
		return;
	}

	if (m_velocity.z >= Constant::One_F && m_velocity.LengthSquared() == Constant::One_F && !m_bJump)
	{
		if (m_animeType != AnimeType::StrafeRunFront)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunFront>(this));
			m_animeType = AnimeType::StrafeRunFront;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_bJumpLanded = true;
		return;
	}

	if (m_velocity.z <= -Constant::One_F && m_velocity.LengthSquared() == Constant::One_F && !m_bJump)
	{
		if (m_animeType != AnimeType::StrafeRunBack)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunBack>(this));
			m_animeType = AnimeType::StrafeRunBack;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("BackRunAttenuation").get<float>();
		m_bJumpLanded = true;
		return;
	}

	if (m_velocity.x >= Constant::One_F && m_velocity.LengthSquared() == Constant::One_F && !m_bJump)
	{
		if (m_animeType != AnimeType::StrafeRunRight)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunRight>(this));
			m_animeType = AnimeType::StrafeRunRight;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("LRRunAttenuation").get<float>();
		m_bJumpLanded = true;
		return;
	}

	if (m_velocity.x <= -Constant::One_F && m_velocity.LengthSquared() == Constant::One_F && !m_bJump)
	{
		if (m_animeType != AnimeType::StrafeRunLeft)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunLeft>(this));
			m_animeType = AnimeType::StrafeRunLeft;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("LRRunAttenuation").get<float>();
		m_bJumpLanded = true;
		return;
	}

	if (m_velocity.z >= Constant::One_F && m_velocity.x >= Constant::One_F && !m_bJump)
	{
		if (m_animeType != AnimeType::StrafeRunRight45)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunRight45>(this));
			m_animeType = AnimeType::StrafeRunRight45;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("DiagonalFrontRunAttenuation").get<float>();
		m_bJumpLanded = true;
		return;
	}

	if (m_velocity.z <= -Constant::One_F && m_velocity.x >= Constant::One_F && !m_bJump)
	{
		if (m_animeType != AnimeType::StrafeRunRight135)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunRight135>(this));
			m_animeType = AnimeType::StrafeRunRight135;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("DiagonalBackRunAttenuation").get<float>();
		m_bJumpLanded = true;
		return;
	}

	if (m_velocity.z >= Constant::One_F && m_velocity.x <= -Constant::One_F && !m_bJump)
	{
		if (m_animeType != AnimeType::StrafeRunLeft45)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunLeft45>(this));
			m_animeType = AnimeType::StrafeRunLeft45;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("DiagonalFrontRunAttenuation").get<float>();
		m_bJumpLanded = true;
		return;
	}

	if (m_velocity.z <= -Constant::One_F && m_velocity.x <= -Constant::One_F && !m_bJump)
	{
		if (m_animeType != AnimeType::StrafeRunLeft135)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunLeft135>(this));
			m_animeType = AnimeType::StrafeRunLeft135;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("DiagonalBackRunAttenuation").get<float>();
		m_bJumpLanded = true;
		return;
	}

	if (m_bJump)
	{
		if (m_jumpPower > Constant::Zero_F)
		{
			if (m_animeType != AnimeType::JumpStart)
			{
				m_upStateContext->SetNextState(std::make_unique<JumpStart>(this));
				m_animeType = AnimeType::JumpStart;
				m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
			}
		}
		return;
	}
	else
	{
		if (!m_velocity.LengthSquared() && m_bJumpLanded)
		{
			if (m_animeType != AnimeType::Idle)
			{
				m_upStateContext->SetNextState(std::make_unique<Idle>(this));
				m_animeType = AnimeType::Idle;
				m_saveAnimeType = AnimeType::Idle;
			}
		}
		else if (m_animeType != AnimeType::JumpLanding)
		{
			m_upStateContext->SetNextState(std::make_unique<JumpLanding>(this));
			m_animeType = AnimeType::JumpLanding;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		else if (!m_bJumpLanded)
		{
			if (m_animationBlend.GetAnimator()->IsAnimationEnd())
			{
				m_bJumpLanded = true;
			}
		}
	}
}

bool Player::UpdateAnimationAttack()
{
	float nowAnimeTime;

	if (m_animeType == AnimeType::Idle)
	{
		nowAnimeTime = Constant::Zero_F;
	}
	else
	{
		nowAnimeTime = m_animationBlend.GetAnimator()->GetNowAnimationTime();
	}
	float nowAnimeMaxTime = m_animationBlend.GetAnimator()->GetNowAnimationMaxTime();
	float animeRemainingTime = nowAnimeMaxTime - nowAnimeTime;

	if (m_wpKeyManager.lock()->GetKeyState(VK_LBUTTON) && !m_bJump)
	{
		if (m_animeType != AnimeType::Combo01 && m_animeType != AnimeType::Combo02 && !m_animationBlend.GetBlend())
		{
			if (m_animeType == AnimeType::Combo03)
			{
				if (animeRemainingTime > m_wpJson.lock()->GetConstJson().at("CanChangeTime").at("Combo01").get<float>())
				{
					m_velocity = Math::Vector3::Zero;
					return false;
				}
			}
			m_upStateContext->SetNextState(std::make_unique<Combo01>(this));
			m_animeType = AnimeType::Combo01;
		}
		else if (m_animeType == AnimeType::Combo01 && m_animeType != AnimeType::Combo02 && m_animeType != AnimeType::Combo03)
		{
			if (animeRemainingTime > m_wpJson.lock()->GetConstJson().at("CanChangeTime").at("Combo02").get<float>())
			{
				m_velocity = Math::Vector3::Zero;
				return false;
			}
			m_upStateContext->SetNextState(std::make_unique<Combo02>(this));
			m_animeType = AnimeType::Combo02;
		}
		else if (m_animeType != AnimeType::Combo01 && m_animeType == AnimeType::Combo02 && m_animeType != AnimeType::Combo03)
		{
			if (animeRemainingTime > m_wpJson.lock()->GetConstJson().at("CanChangeTime").at("Combo03").get<float>())
			{
				m_velocity = Math::Vector3::Zero;
				return false;
			}
			m_upStateContext->SetNextState(std::make_unique<Combo03>(this));
			m_animeType = AnimeType::Combo03;
		}
	}

	if (nowAnimeTime < m_wpJson.lock()->GetConstJson().at("CanRunTime").at("Combo01").get<float>() && m_animeType == AnimeType::Combo01)
	{
		m_velocity = Math::Vector3::Zero;
		m_bJumpLanded = true;
		return false;
	}

	if (nowAnimeTime < m_wpJson.lock()->GetConstJson().at("CanRunTime").at("Combo02").get<float>() && m_animeType == AnimeType::Combo02)
	{
		m_velocity = Math::Vector3::Zero;
		m_bJumpLanded = true;
		return false;
	}

	if (nowAnimeTime < m_wpJson.lock()->GetConstJson().at("CanRunTime").at("Combo03").get<float>() && m_animeType == AnimeType::Combo03)
	{
		m_velocity = Math::Vector3::Zero;
		m_bJumpLanded = true;
		return false;
	}

	if (m_wpKeyManager.lock()->GetKeyState(VK_LBUTTON) && m_bJump && !m_bJumpAttack)
	{
		if (m_animeType != AnimeType::AirCombo01 && m_animeType != AnimeType::AirCombo02 && !m_animationBlend.GetBlend())
		{
			if (m_animeType == AnimeType::AirCombo03)
			{
				if (animeRemainingTime > m_wpJson.lock()->GetConstJson().at("CanChangeTime").at("AirCombo01").get<float>())
				{
					m_verticalVector = Constant::Zero_F;
					m_velocity = Math::Vector3::Zero;
					return false;
				}
			}
			m_upStateContext->SetNextState(std::make_unique<AirCombo01>(this));
			m_animeType = AnimeType::AirCombo01;
		}
		else if (m_animeType == AnimeType::AirCombo01 && m_animeType != AnimeType::AirCombo02 && m_animeType != AnimeType::AirCombo03)
		{
			if (animeRemainingTime > m_wpJson.lock()->GetConstJson().at("CanChangeTime").at("AirCombo02").get<float>())
			{
				m_verticalVector = Constant::Zero_F;
				m_velocity = Math::Vector3::Zero;
				return false;
			}
			m_upStateContext->SetNextState(std::make_unique<AirCombo02>(this));
			m_animeType = AnimeType::AirCombo02;
		}
		else if (m_animeType != AnimeType::AirCombo01 && m_animeType == AnimeType::AirCombo02 && m_animeType != AnimeType::AirCombo03)
		{
			if (animeRemainingTime > m_wpJson.lock()->GetConstJson().at("CanChangeTime").at("AirCombo03").get<float>())
			{
				m_verticalVector = Constant::Zero_F;
				m_velocity = Math::Vector3::Zero;
				return false;
			}
			m_upStateContext->SetNextState(std::make_unique<AirCombo03>(this));
			m_animeType = AnimeType::AirCombo03;
			m_bJumpAttack = true;
		}
	}

	if (nowAnimeTime < m_wpJson.lock()->GetConstJson().at("CanRunTime").at("AirCombo01").get<float>() && m_animeType == AnimeType::AirCombo01)
	{
		m_verticalVector = Constant::Zero_F;
		m_velocity = Math::Vector3::Zero;
		return false;
	}

	if (nowAnimeTime < m_wpJson.lock()->GetConstJson().at("CanRunTime").at("AirCombo02").get<float>() && m_animeType == AnimeType::AirCombo02)
	{
		m_verticalVector = Constant::Zero_F;
		m_velocity = Math::Vector3::Zero;
		return false;
	}

	if (nowAnimeTime < m_wpJson.lock()->GetConstJson().at("CanRunTime").at("AirCombo03").get<float>() && m_animeType == AnimeType::AirCombo03)
	{
		m_verticalVector = Constant::Zero_F;
		m_velocity = Math::Vector3::Zero;
		return false;
	}

	return true;
}

void Player::UpdateAnimationDodge()
{
	if (m_wpKeyManager.lock()->GetKeyState('Q'))
	{
		m_limPower = Constant::One_F;
	}

	if (m_bJustDodge)
	{
		if (m_animeType != AnimeType::DodgeBack)
		{
			m_upStateContext->SetNextState(std::make_unique<DodgeBack>(this));
			m_animeType = AnimeType::DodgeBack;
		}

		if (m_dissolveRate < Constant::One_F)
		{
			m_dissolveRate += m_wpJson.lock()->GetConstJson().at("DissolveRateSpeed").get<float>();
		}

		m_velocity.z = -Constant::Two_F;

		m_justDodgeSpeedCount += m_wpJson.lock()->GetConstJson().at("JustDodgeSpeed").get<float>();

		if (m_justDodgeSpeedCount > Constant::One_F)
		{
			m_bJustDodge = false;
			m_justDodgeSpeedCount = Constant::Zero_F;
		}
	}
	else
	{
		if (m_dissolveRate > Constant::Zero_F)
		{
			m_dissolveRate -= m_wpJson.lock()->GetConstJson().at("DissolveRateSpeed").get<float>();
		}
		else
		{
			m_dissolveRate = Constant::Zero_F;
		}
	}
}

void Player::UpdateCollision()
{
	DirectX::BoundingSphere bs;
	bs.Center = m_transform.Positon();
	bs.Center.y += m_wpJson.lock()->GetConstJson().at("BumpHeight");
	bs.Radius = m_wpJson.lock()->GetConstJson().at("HitToRadius");

	for (const auto& wpEnemy : Application::Instance().GetSceneManager()->GetCurrentScene()->GetEnemyManager()->GetEnemyObjects())
	{
		std::list<Collider::CollisionResult> collisionResults;

		if (wpEnemy.lock()->GetCollider()->Intersects(Collider::Type::TypeBump, bs, &collisionResults))
		{
			for (const Collider::CollisionResult& result : collisionResults)
			{
				Math::Vector3 pushVec = result.m_hitDir * result.m_overlapDistance;

				m_transform.Positon(m_transform.Positon() + pushVec);
			}
		}
	}

	for (const auto& wpEnvironment : Application::Instance().GetSceneManager()->GetCurrentScene()->GetEnvironmentManager()->GetEnvironmentObjects())
	{
		std::list<Collider::CollisionResult> collisionResults;

		if (!wpEnvironment.lock()->GetCollider())continue;

		wpEnvironment.lock()->GetCollider()->Intersects(Collider::Type::TypeBump, bs, &collisionResults);

		if (collisionResults.size())
		{
			Collider::CollisionResult pNearestRes;

			for (const Collider::CollisionResult& result : collisionResults)
			{
				Math::Vector3 pushVec = result.m_hitDir * result.m_overlapDistance;
				pushVec.y = Constant::Zero_F;
				m_transform.Positon(m_transform.Positon() + pushVec);
			}
		}
	}

	Collider::RayInfo ray;
	ray.m_pos = m_transform.Positon();
	ray.m_dir = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("RayDir").at("X").get<float>(),
		m_wpJson.lock()->GetConstJson().at("RayDir").at("Y").get<float>(),
		m_wpJson.lock()->GetConstJson().at("RayDir").at("Z").get<float>());
	ray.m_range = m_wpJson.lock()->GetConstJson().at("RayRange").get<float>();
	ray.m_pos.y += ray.m_range + m_velocity.y;

	std::list<Collider::CollisionResult> groundResults;

	for (const auto& wpEnvironment : Application::Instance().GetSceneManager()->GetCurrentScene()->GetEnvironmentManager()->GetEnvironmentObjects())
	{
		if (!wpEnvironment.lock()->GetCollider())continue;

		wpEnvironment.lock()->GetCollider()->Intersects(Collider::Type::TypeGround, ray, &groundResults);
	}

	if (groundResults.size())
	{
		Collider::CollisionResult nearestRes;

		for (const Collider::CollisionResult& result : groundResults)
		{
			if (result.m_hit && result.m_overlapDistance > nearestRes.m_overlapDistance)
			{
				nearestRes = result;
			}
		}

		if (nearestRes.m_hit)
		{
			m_transform.Positon(nearestRes.m_hitPos);
			m_verticalVector = Constant::Zero_F;

			if (m_bJump)
			{
				m_jumpPower = Constant::Zero_F;
				m_velocity.y = Constant::Zero_F;
				m_bJump = false;
				m_bJumpLanded = false;
				m_bJumpAttack = false;
			}
		}
	}
}

void Player::UpdateRotate()
{
	Math::Vector3 nowDir;

	switch (m_animeType)
	{
	case AnimeType::RunFast:
	case AnimeType::JumpStart:
	case AnimeType::JumpLanding:
	case AnimeType::AirCombo01:
	case AnimeType::AirCombo02:
	case AnimeType::AirCombo03:
		nowDir = m_transform.Matrix().Forward();
		break;
	case AnimeType::StrafeRunBack:
	case AnimeType::DodgeBack:
		nowDir = m_transform.Matrix().Backward();
		break;
	case AnimeType::StrafeRunFront:
		nowDir = m_transform.Matrix().Forward();
		break;
	case AnimeType::StrafeRunLeft:
		nowDir = m_transform.Matrix().Right();
		break;
	case AnimeType::StrafeRunLeft45:
		nowDir = m_transform.Matrix().Right();
		nowDir = Math::Vector3::Transform(nowDir, Math::Matrix::CreateRotationY(static_cast<float>(M_PI_4)));
		break;
	case AnimeType::StrafeRunLeft135:
		nowDir = m_transform.Matrix().Right();
		nowDir = Math::Vector3::Transform(nowDir, Math::Matrix::CreateRotationY(-static_cast<float>(M_PI_4)));
		break;
	case AnimeType::StrafeRunRight:
		nowDir = m_transform.Matrix().Left();
		break;
	case AnimeType::StrafeRunRight45:
		nowDir = m_transform.Matrix().Left();
		nowDir = Math::Vector3::Transform(nowDir, Math::Matrix::CreateRotationY(-static_cast<float>(M_PI_4)));
		break;
	case AnimeType::StrafeRunRight135:
		nowDir = m_transform.Matrix().Left();
		nowDir = Math::Vector3::Transform(nowDir, Math::Matrix::CreateRotationY(static_cast<float>(M_PI_4)));
		break;
	}

	Math::Vector3 targetDir = m_velocity;

	nowDir.Normalize();
	targetDir.Normalize();

	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng *= ToDegrees;

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng *= ToDegrees;

	float betweenAng = targetAng - nowAng;

	if (betweenAng > static_cast<float>(M_PI * ToDegrees))
	{
		betweenAng -= static_cast<float>(M_PI) * Constant::Two_F * ToDegrees;
	}
	else if (betweenAng < -static_cast<float>(M_PI) * ToDegrees)
	{
		betweenAng += static_cast<float>(M_PI) * Constant::Two_F * ToDegrees;
	}

	m_yaw += std::clamp(betweenAng, -m_wpJson.lock()->GetConstJson().at("LookBackSpeed").get<float>(), m_wpJson.lock()->GetConstJson().at("LookBackSpeed").get<float>());

	if (m_yaw > static_cast<float>(M_PI) * Constant::Two_F * ToDegrees)
	{
		m_yaw -= static_cast<float>(M_PI) * Constant::Two_F * ToDegrees;
	}

	if (m_yaw < -static_cast<float>(M_PI) * Constant::Two_F * ToDegrees)
	{
		m_yaw += static_cast<float>(M_PI) * Constant::Two_F * ToDegrees;
	}
}

void Player::UpdateMatrix()
{
	m_velocity.x *= m_wpJson.lock()->GetConstJson().at("VelocityScalar").get<float>();
	m_velocity.z *= m_wpJson.lock()->GetConstJson().at("VelocityScalar").get<float>();

	m_transform.Positon(m_transform.Positon() + m_velocity);

	m_transform.Scale(m_wpJson.lock()->GetConstJson().at("ScaleSize").get<float>());

	m_velocity.y = Constant::Zero_F;

	if (m_velocity.LengthSquared())
	{
		UpdateRotate();
	}

	m_transform.RotationY(m_yaw);

	m_transform.MatrixComposition();
}

void Player::UpdateCamera()
{
	m_wpCameraManager.lock()->SetCamera(m_wpCamera.lock());

	m_mCamWork.resize(m_wpJson.lock()->GetConstJson().at("CameraLateFrame").get<int>());

	for (int i = Constant::Zero_I; i < m_wpJson.lock()->GetConstJson().at("CameraLateFrame").get<int>() - Constant::One_I; ++i)
	{
		m_mCamWork[i] = m_mCamWork[i + Constant::One_I];
	}

	m_mCamWork[m_wpJson.lock()->GetConstJson().at("CameraLateFrame").get<int>() - Constant::One_I] = m_wpCamera.lock()->GetCameraMatrix();

	Math::Matrix mLerp = Math::Matrix::Lerp(m_mCamWork[Constant::Zero_I], m_transform.Matrix(), m_wpJson.lock()->GetConstJson().at("CameraLerpSpeed").get<float>());

	m_wpCameraManager.lock()->SetMatrix(mLerp);
}

void Player::ImGuiUpdateGameObject()
{

}

void Player::ReferenceJson()
{
	m_wpJsonManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Player");
}

Player::Combo01::Combo01(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("Combo01"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedAttack").get<float>());
}

Player::Combo02::Combo02(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("Combo02"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedAttack").get<float>());
}

Player::Combo03::Combo03(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("Combo03"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedAttack").get<float>());
}

Player::AirCombo01::AirCombo01(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("AirCombo01"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedAttack").get<float>());
}

Player::AirCombo02::AirCombo02(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("AirCombo02"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedAttack").get<float>());
}

Player::AirCombo03::AirCombo03(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("AirCombo03"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedAttack").get<float>());
}

Player::Idle::Idle(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("Idle"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("BecomeIdleSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Player::KnockBack01::KnockBack01(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("KnockDown01"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedKnockDown").get<float>());
}

Player::Rise01::Rise01(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("Rise01"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedRise").get<float>());
}

Player::RunFast::RunFast(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("RunFast"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedRunFast").get<float>());
}

Player::StrafeRunFront::StrafeRunFront(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunFront"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Player::StrafeRunBack::StrafeRunBack(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunBack"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Player::StrafeRunLeft::StrafeRunLeft(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunLeft"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Player::StrafeRunRight::StrafeRunRight(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunRight"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Player::StrafeRunRight45::StrafeRunRight45(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunRight45"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Player::StrafeRunRight135::StrafeRunRight135(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunRight135"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Player::StrafeRunLeft45::StrafeRunLeft45(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunLeft45"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Player::StrafeRunLeft135::StrafeRunLeft135(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunLeft135"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Player::JumpStart::JumpStart(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("JumpStart"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedJump").get<float>());
}

Player::JumpLanding::JumpLanding(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("JumpLanding"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedJump").get<float>());
}

Player::DodgeBack::DodgeBack(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("DodgeBack"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationChangeSpeed").get<float>());
	pPlayer->SetAnimeSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeedBackStep").get<float>());
}