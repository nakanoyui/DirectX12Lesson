#include "Sword.h"

#include "Application/main.h"
#include "Application/GameSystem/GameSystem.h"

#include "../../CharacterObject/PlayerObject/PlayerManager.h"
#include "../../CharacterObject/EnemyObject/EnemyManager.h"

#include "../../CharacterObject/EnemyObject/EnemyObject.h"

void Sword::LoadAsset()
{
	ReferenceJson();

	m_wpModelManager = Application::Instance().GetGameSystem()->GetModelManager();
	m_wpModelManager.lock()->ResisterModelWork(m_wpJsonManager.lock()->GetAssetJson().at("Sword").at("Model"));
	m_spModelWork = m_wpModelManager.lock()->GetModelWork("Sword.gltf");
}

void Sword::Init()
{
	m_wpPlayerManager = Application::Instance().GetGameSystem()->GetPlayerManager();
	m_wpShaderManager = Application::Instance().GetGameSystem()->GetShaderManager();

	m_upDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upDebugWireFrame->CreateLineCube(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("X").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Y").get<float>() * Constant::Two_F,
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Z").get<float>() * Constant::Two_F), BlueColor);
}

void Sword::Update()
{
	const auto& workNodes = m_wpPlayerManager.lock()->GetPlayer().lock()->GetModelWork()->GetNodes();

	for (auto&& nodeIdx : m_wpPlayerManager.lock()->GetPlayer().lock()->GetModelWork()->GetData()->GetBoneNodeIndices())
	{
		const auto& dataNode = m_wpPlayerManager.lock()->GetPlayer().lock()->GetModelWork()->GetData()->GetOriginalNodes()[nodeIdx];
		const auto& workNode = workNodes[nodeIdx];

		if (dataNode.m_name == m_wpJson.lock()->GetConstJson().at("HoldPlace"))
		{
			Math::Matrix mWork = workNode.m_worldTransform;
			Math::Vector3 scale;
			Math::Quaternion quaternion;
			Math::Vector3 pos;
			mWork.Decompose(scale, quaternion, pos);
			m_transform.Scale(scale);
			m_transform.Quaternion(quaternion);
			m_transform.Positon(pos);
			break;
		}
	}

	m_transform.MatrixComposition();
	m_transform.Matrix(m_transform.Matrix() * m_wpPlayerManager.lock()->GetPlayer().lock()->GetMatrix());

	UpdateCollision();
}

void Sword::DrawStandard()
{
	m_wpShaderManager.lock()->GetShader("StandardShader")->DrawModel(*m_spModelWork, m_transform.Matrix());
}

void Sword::DrawEffect()
{
	Math::Matrix mWork = m_transform.Matrix();
	Math::Vector3 scale;
	Math::Quaternion quaternion;
	Math::Vector3 pos;
	mWork.Decompose(scale, quaternion, pos);

	Math::Matrix mRotate = Math::Matrix::CreateFromQuaternion(quaternion);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(pos);
	Math::Matrix mLocal = Math::Matrix::CreateTranslation(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Z")));

	m_wpShaderManager.lock()->GetShader("EffectShader")->DrawEffect(m_upDebugWireFrame->GetMesh().get(), mLocal * mRotate * mTrans);
}

void Sword::UpdateCollision()
{
	if (m_wpPlayerManager.lock()->GetPlayer().lock()->GetAnimeType() < Player::AnimeType::Combo01 ||
		m_wpPlayerManager.lock()->GetPlayer().lock()->GetAnimeType() > Player::AnimeType::Combo03)
	{
		return;
	}

	if (m_wpPlayerManager.lock()->GetPlayer().lock()->GetAnimeType() == Player::AnimeType::Combo01 &&
		m_wpPlayerManager.lock()->GetPlayer().lock()->GetAnimationBlend().GetAnimator()->GetNowAnimationTime() < m_wpJson.lock()->GetConstJson().at("CanAttackTime").at("Combo01") ||
		m_wpPlayerManager.lock()->GetPlayer().lock()->GetAnimeType() == Player::AnimeType::Combo02 &&
		m_wpPlayerManager.lock()->GetPlayer().lock()->GetAnimationBlend().GetAnimator()->GetNowAnimationTime() < m_wpJson.lock()->GetConstJson().at("CanAttackTime").at("Combo02") ||
		m_wpPlayerManager.lock()->GetPlayer().lock()->GetAnimeType() == Player::AnimeType::Combo03 &&
		m_wpPlayerManager.lock()->GetPlayer().lock()->GetAnimationBlend().GetAnimator()->GetNowAnimationTime() < m_wpJson.lock()->GetConstJson().at("CanAttackTime").at("Combo03"))
	{
		return;
	}

	Math::Matrix work = m_transform.Matrix();
	Math::Vector3 scale;
	Math::Quaternion quaternion;
	Math::Vector3 pos;
	work.Decompose(scale, quaternion, pos);

	Math::Matrix mRotate = Math::Matrix::CreateFromQuaternion(quaternion);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(pos);
	Math::Matrix mLocal = Math::Matrix::CreateTranslation(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Z")));

	DirectX::BoundingBox aabb;
	aabb.Transform(aabb, mLocal * mRotate * mTrans);
	aabb.Extents = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("X").get<float>(),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Y").get<float>(),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Z").get<float>());

	for (const auto& wpEnemy : Application::Instance().GetGameSystem()->GetEnemyManager()->GetEnemyObjects())
	{
		if (wpEnemy.lock()->GetCollider()->Intersects(Collider::Type::TypeAttack, aabb, nullptr))
		{
			switch (m_wpPlayerManager.lock()->GetPlayer().lock()->GetAnimeType())
			{
			case Player::AnimeType::Combo01:
				wpEnemy.lock()->SetDamageType(EnemyObject::DamageType::KnockBack01);
				break;
			case Player::AnimeType::Combo02:
				wpEnemy.lock()->SetDamageType(EnemyObject::DamageType::KnockBack02);
				break;
			case Player::AnimeType::Combo03:
				wpEnemy.lock()->SetDamageType(EnemyObject::DamageType::BlowAway);
				break;
			default:
				break;
			}

			m_wpPlayerManager.lock()->GetPlayer().lock()->SetHitStop(true);
		}
		else
		{
			wpEnemy.lock()->SetDamageType(EnemyObject::DamageType::None);
		}
	}
}

void Sword::ImGuiUpdate()
{
}

void Sword::ReferenceJson()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Sword");
}