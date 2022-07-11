#include "Player.h"

#include "../EnemyObject/EnemyManager.h"

void Player::LoadAsset()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Player");

	m_spModelWork = std::make_shared<ModelWork>();
	m_spModelWork->Load(m_wpJsonManager.lock()->GetAssetJson().at("Player").at("Model"));
}

void Player::Init()
{
	m_wpShaderManager = Application::Instance().GetGameSystem()->GetShaderManager();
	m_wpCameraManager = Application::Instance().GetGameSystem()->GetCameraManager();
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

	m_upStateContext = std::make_unique<AnimationStateContext>(this);

	m_animeType = AnimeType::Idle;
	m_animationBlend.SetAnimation(m_spModelWork->GetData()->GetAnimation("Idle"));
	m_animationBlend.SetSpeed(m_wpJson.lock()->GetConstJson().at("AnimationSpeed").get<float>());

	m_upDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upDebugWireFrame->CreateLineSphere(m_wpJson.lock()->GetConstJson().at("HitToRadius").get<float>(), 50, BlueColor);
}

void Player::Update()
{
	m_velocity = Math::Vector3::Zero;

	if (m_wpKeyManager.lock()->GetKeyState('W', true))
	{
		m_velocity.z = Constant::One_F;
	}

	if (m_wpKeyManager.lock()->GetKeyState('S', true))
	{
		m_velocity.z = -Constant::One_F;
	}

	if (m_wpKeyManager.lock()->GetKeyState('D', true))
	{
		m_velocity.x = Constant::One_F;
	}

	if (m_wpKeyManager.lock()->GetKeyState('A', true))
	{
		m_velocity.x = -Constant::One_F;
	}

	//if (m_wpKeyManager.lock()->GetKeyState(VK_SPACE) && !m_bJump)
	//{
	//	m_jumpPower = 1.5f;
	//	m_bJump = true;
	//}

	//m_velocity.y += m_jumpPower;

	//if (m_mWorld.Translation().y + m_velocity.y > 0)
	//{
	//	m_jumpPower -= 0.15f;
	//}
	//else
	//{
	//	auto pos = m_mWorld.Translation();
	//	pos.y = 0.0f;
	//	m_mWorld.Translation(pos);
	//	m_jumpPower = 0.0f;
	//	m_bJump = false;
	//}

	UpdateAnimation();

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

	m_animationBlend.AdvanceTime(m_spModelWork->WorkNodes());
	m_spModelWork->CalcNodeMatrices();

	UpdateCamera();
}

void Player::DrawSkinMesh()
{
	m_wpShaderManager.lock()->GetShader("SkinMeshShader")->DrawModel(*m_spModelWork, m_mWorld);
}

void Player::DrawEffect()
{
	Math::Vector3 offset;
	offset.y = m_wpJson.lock()->GetConstJson().at("BumpHeight");
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_mWorld.Translation());
	m_wpShaderManager.lock()->GetShader("EffectShader")->DrawEffect(m_upDebugWireFrame->GetMesh().get(), mTrans, offset);
}

void Player::UpdateAnimation()
{
	float nowAnimeTime = m_animationBlend.GetAnimator()->GetNowAnimationTime();
	float nowAnimeMaxTime = m_animationBlend.GetAnimator()->GetNowAnimationMaxTime();
	float animeRemainingTime = nowAnimeMaxTime - nowAnimeTime;

	if (m_wpKeyManager.lock()->GetKeyState(VK_LBUTTON))
	{
		if (m_animeType != AnimeType::Combo01 && m_animeType != AnimeType::Combo02)
		{
			if (animeRemainingTime > m_wpJson.lock()->GetConstJson().at("CanChangeTime").at("Combo01").get<float>())return;
			m_upStateContext->SetNextState(std::make_unique<Combo01>(this));
			m_animeType = AnimeType::Combo01;
		}
		else if (m_animeType == AnimeType::Combo01 && m_animeType != AnimeType::Combo02 && m_animeType != AnimeType::Combo03)
		{
			if (animeRemainingTime > m_wpJson.lock()->GetConstJson().at("CanChangeTime").at("Combo02").get<float>())return;
			m_upStateContext->SetNextState(std::make_unique<Combo02>(this));
			m_animeType = AnimeType::Combo02;
		}
		else if (m_animeType != AnimeType::Combo01 && m_animeType == AnimeType::Combo02 && m_animeType != AnimeType::Combo03)
		{
			if (animeRemainingTime > m_wpJson.lock()->GetConstJson().at("CanChangeTime").at("Combo03").get<float>())return;
			m_upStateContext->SetNextState(std::make_unique<Combo03>(this));
			m_animeType = AnimeType::Combo03;
		}
	}

	if (nowAnimeTime < m_wpJson.lock()->GetConstJson().at("CanRunTime").at("Combo01").get<float>() && m_animeType == AnimeType::Combo01)
	{
		m_velocity = Math::Vector3::Zero;
		return;
	}

	if (nowAnimeTime < m_wpJson.lock()->GetConstJson().at("CanRunTime").at("Combo02").get<float>() && m_animeType == AnimeType::Combo02)
	{
		m_velocity = Math::Vector3::Zero;
		return;
	}

	if (nowAnimeTime < m_wpJson.lock()->GetConstJson().at("CanRunTime").at("Combo03").get<float>() && m_animeType == AnimeType::Combo03)
	{
		m_velocity = Math::Vector3::Zero;
		return;
	}

	if (m_velocity.z >= Constant::One_F && m_velocity.LengthSquared() == Constant::One_F)
	{
		if (m_animeType != AnimeType::StrafeRunFront)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunFront>(this));
			m_animeType = AnimeType::StrafeRunFront;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		return;
	}

	if (m_velocity.z <= -Constant::One_F && m_velocity.LengthSquared() == Constant::One_F)
	{
		if (m_animeType != AnimeType::StrafeRunBack)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunBack>(this));
			m_animeType = AnimeType::StrafeRunBack;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("BackRunAttenuation").get<float>();
		return;
	}

	if (m_velocity.x >= Constant::One_F && m_velocity.LengthSquared() == Constant::One_F)
	{
		if (m_animeType != AnimeType::StrafeRunRight)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunRight>(this));
			m_animeType = AnimeType::StrafeRunRight;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("LRRunAttenuation").get<float>();
		return;
	}

	if (m_velocity.x <= -Constant::One_F && m_velocity.LengthSquared() == Constant::One_F)
	{
		if (m_animeType != AnimeType::StrafeRunLeft)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunLeft>(this));
			m_animeType = AnimeType::StrafeRunLeft;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("LRRunAttenuation").get<float>();
		return;
	}

	if (m_velocity.z >= Constant::One_F && m_velocity.x >= Constant::One_F)
	{
		if (m_animeType != AnimeType::StrafeRunRight45)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunRight45>(this));
			m_animeType = AnimeType::StrafeRunRight45;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("DiagonalFrontRunAttenuation").get<float>();
		return;
	}

	if (m_velocity.z <= -Constant::One_F && m_velocity.x >= Constant::One_F)
	{
		if (m_animeType != AnimeType::StrafeRunRight135)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunRight135>(this));
			m_animeType = AnimeType::StrafeRunRight135;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("DiagonalBackRunAttenuation").get<float>();
		return;
	}

	if (m_velocity.z >= Constant::One_F && m_velocity.x <= -Constant::One_F)
	{
		if (m_animeType != AnimeType::StrafeRunLeft45)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunLeft45>(this));
			m_animeType = AnimeType::StrafeRunLeft45;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("DiagonalFrontRunAttenuation").get<float>();
		return;
	}

	if (m_velocity.z <= -Constant::One_F && m_velocity.x <= -Constant::One_F)
	{
		if (m_animeType != AnimeType::StrafeRunLeft135)
		{
			m_upStateContext->SetNextState(std::make_unique<StrafeRunLeft135>(this));
			m_animeType = AnimeType::StrafeRunLeft135;
			m_speed = m_wpJson.lock()->GetConstJson().at("ResetSpeed").get<float>();
		}
		m_velocity *= m_wpJson.lock()->GetConstJson().at("DiagonalBackRunAttenuation").get<float>();
		return;
	}

	if (!m_velocity.LengthSquared())
	{
		if (m_animeType != AnimeType::Idle)
		{
			m_upStateContext->SetNextState(std::make_unique<Idle>(this));
			m_animeType = AnimeType::Idle;
		}
	}

	//if (m_velocity.y != 0)
	//{
	//	//m_animeType = AnimeType::Jump;
	//}
}

void Player::UpdateCollision()
{
	DirectX::BoundingSphere bs;
	bs.Center = m_mWorld.Translation();
	bs.Center.y += m_wpJson.lock()->GetConstJson().at("BumpHeight");
	bs.Radius = m_wpJson.lock()->GetConstJson().at("HitToRadius");

	for (const auto& wpEnemy : Application::Instance().GetGameSystem()->GetEnemyManager()->GetEnemyObjects())
	{
		std::list<Collider::CollisionResult> collisionResults;

		if (wpEnemy.lock()->GetCollider()->Intersects(Collider::Type::TypeBump, bs, &collisionResults))
		{
			for (const Collider::CollisionResult& result : collisionResults)
			{
				Math::Vector3 pushVec = result.m_hitDir * result.m_overlapDistance;

				m_velocity += pushVec;
			}
		}
	}
}

void Player::UpdateRotate()
{
	Math::Vector3 nowDir;

	switch (m_animeType)
	{
	case AnimeType::StrafeRunBack:
		nowDir = m_mWorld.Backward();
		break;
	case AnimeType::StrafeRunFront:
		nowDir = m_mWorld.Forward();
		break;
	case AnimeType::StrafeRunLeft:
		nowDir = m_mWorld.Right();
		break;
	case AnimeType::StrafeRunLeft45:
		nowDir = m_mWorld.Right();
		nowDir = Math::Vector3::Transform(nowDir, Math::Matrix::CreateRotationY(static_cast<float>(M_PI_4)));
		break;
	case AnimeType::StrafeRunLeft135:
		nowDir = m_mWorld.Right();
		nowDir = Math::Vector3::Transform(nowDir, Math::Matrix::CreateRotationY(-static_cast<float>(M_PI_4)));
		break;
	case AnimeType::StrafeRunRight:
		nowDir = m_mWorld.Left();
		break;
	case AnimeType::StrafeRunRight45:
		nowDir = m_mWorld.Left();
		nowDir = Math::Vector3::Transform(nowDir, Math::Matrix::CreateRotationY(-static_cast<float>(M_PI_4)));
		break;
	case AnimeType::StrafeRunRight135:
		nowDir = m_mWorld.Left();
		nowDir = Math::Vector3::Transform(nowDir, Math::Matrix::CreateRotationY(static_cast<float>(M_PI_4)));
		break;
	}

	Math::Vector3 targetDir = m_velocity;

	nowDir.Normalize();
	targetDir.Normalize();

	// DegreeŠp
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// ŠÔ‚ÌŠp“x
	float betweenAng = targetAng - nowAng;

	if (betweenAng > DirectX::XMConvertToDegrees(static_cast<float>(M_PI)))
	{
		betweenAng -= DirectX::XMConvertToDegrees(static_cast<float>(M_2_PI));
	}
	else if (betweenAng < -DirectX::XMConvertToDegrees(static_cast<float>(M_PI)))
	{
		betweenAng += DirectX::XMConvertToDegrees(static_cast<float>(M_2_PI));
	}

	float rotAng = std::clamp(betweenAng, -m_wpJson.lock()->GetConstJson().at("LookBackSpeed").get<float>(), m_wpJson.lock()->GetConstJson().at("LookBackSpeed").get<float>());

	m_yaw += rotAng;
}

void Player::UpdateMatrix()
{
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_mWorld.Translation() + m_velocity);

	Math::Matrix mScale = Math::Matrix::CreateScale(m_wpJson.lock()->GetConstJson().at("ScaleSize").get<float>());

	Math::Matrix mRotate;

	if (m_velocity.LengthSquared())
	{
		UpdateRotate();
	}

	mRotate = Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_yaw),
		DirectX::XMConvertToRadians(Constant::Zero_F),
		DirectX::XMConvertToRadians(Constant::Zero_F));

	m_mWorld = mScale * mRotate * mTrans;
}

void Player::UpdateCamera()
{
	m_wpCameraManager.lock()->SetCamera(m_wpCamera.lock());

	m_mCamWork.resize(m_wpJson.lock()->GetConstJson().at("CameraLateFrame").get<int>());

	for (int i = 0; i < m_wpJson.lock()->GetConstJson().at("CameraLateFrame").get<int>() - Constant::One_I; ++i)
	{
		m_mCamWork[i] = m_mCamWork[i + Constant::One_I];
	}

	m_mCamWork[m_wpJson.lock()->GetConstJson().at("CameraLateFrame").get<int>() - Constant::One_I] = m_wpCamera.lock()->GetCameraMatrix();

	Math::Vector3 lerpPos = Math::Vector3::Lerp(m_mCamWork[Constant::Zero_I].Translation(), m_mWorld.Translation(), m_wpJson.lock()->GetConstJson().at("CameraLerpSpeed").get<float>());

	m_wpCameraManager.lock()->SetMatrix(Math::Matrix::CreateTranslation(lerpPos));
}

void Player::ImGuiUpdate()
{
	ImGui::Text("Speed : %f", m_speed);
}

Combo01::Combo01(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("Combo01"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Combo02::Combo02(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("Combo02"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Combo03::Combo03(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("Combo03"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

Idle::Idle(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("Idle"), false);
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("BecomeIdleSpeed").get<float>());
}

StrafeRunFront::StrafeRunFront(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunFront"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

StrafeRunBack::StrafeRunBack(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunBack"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

StrafeRunLeft::StrafeRunLeft(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunLeft"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

StrafeRunRight::StrafeRunRight(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunRight"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

StrafeRunRight45::StrafeRunRight45(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunRight45"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

StrafeRunRight135::StrafeRunRight135(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunRight135"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

StrafeRunLeft45::StrafeRunLeft45(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunLeft45"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}

StrafeRunLeft135::StrafeRunLeft135(Player* pPlayer)
{
	pPlayer->GetAnimationBlend().SetAnimation(pPlayer->GetModelWork()->GetData()->GetAnimation("StrafeRunLeft135"));
	pPlayer->GetAnimationBlend().SetSpeed(pPlayer->GetJson().lock()->GetConstJson().at("AnimationSpeed").get<float>());
}