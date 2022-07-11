#include "Sword.h"

#include "../../CharacterObject/PlayerObject/PlayerManager.h"
#include "../../CharacterObject/EnemyObject/EnemyManager.h"

void Sword::LoadAsset()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Sword");

	m_spModelData = std::make_shared<ModelData>();
	m_spModelData->Load(m_wpJsonManager.lock()->GetAssetJson().at("Sword").at("Model"));
}

void Sword::Init()
{
	m_wpPlayerManager = Application::Instance().GetGameSystem()->GetPlayerManager();
	m_wpShaderManager = Application::Instance().GetGameSystem()->GetShaderManager();

	m_upDebugWireFrame = std::make_unique<DebugWireFrame>();
	m_upDebugWireFrame->CreateLineCube(Math::Vector3(m_wpJson.lock()->GetConstJson().at("CubeSize").at("X"),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Y"),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Z")), BlueColor);
}

void Sword::Update()
{
	auto& workNodes = m_wpPlayerManager.lock()->GetPlayer().lock()->GetModelWork()->GetNodes();
	// 全ノードから右手のボーンを検索する
	for (auto&& nodeIdx : m_wpPlayerManager.lock()->GetPlayer().lock()->GetModelWork()->GetData()->GetBoneNodeIndices())
	{
		auto& dataNode = m_wpPlayerManager.lock()->GetPlayer().lock()->GetModelWork()->GetData()->GetOriginalNodes()[nodeIdx];
		auto& workNode = workNodes[nodeIdx];

		if (dataNode.m_name == m_wpJson.lock()->GetConstJson().at("HoldPlace"))
		{
			m_mWorld = workNode.m_worldTransform;
			break;
		}
	}

	// プレイヤーの行列を掛けてプレイヤーに追従させる
	m_mWorld = m_mWorld * m_wpPlayerManager.lock()->GetPlayer().lock()->GetMatrix();

	UpdateCollision();
}

void Sword::DrawStandard()
{
	m_wpShaderManager.lock()->GetShader("StandardShader")->DrawModel(*m_spModelData, m_mWorld);
}

void Sword::DrawEffect()
{
	Math::Vector3 scale;
	Math::Vector3 trans;
	Math::Quaternion quaternion;
	m_mWorld.Decompose(scale, quaternion, trans);

	Math::Matrix mRotate = Math::Matrix::CreateFromQuaternion(quaternion);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(trans);
	Math::Matrix mLocal = Math::Matrix::CreateTranslation(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Z")));

	m_wpShaderManager.lock()->GetShader("EffectShader")->DrawEffect(m_upDebugWireFrame->GetMesh().get(), mLocal * mRotate * mTrans);
}

void Sword::UpdateCollision()
{
	Math::Vector3 scale;
	Math::Vector3 trans;
	Math::Quaternion quaternion;
	m_mWorld.Decompose(scale, quaternion, trans);

	Math::Matrix mRotate = Math::Matrix::CreateFromQuaternion(quaternion);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(trans);
	Math::Matrix mLocal = Math::Matrix::CreateTranslation(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("X"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Y"),
		m_wpJson.lock()->GetConstJson().at("LocalPos").at("Z")));

	DirectX::BoundingBox aabb;
	aabb.Transform(aabb, mLocal * mRotate * mTrans);	// ScaleはExtentsで変えるから0でOK
	aabb.Extents = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("X"),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Y"),
		m_wpJson.lock()->GetConstJson().at("CubeSize").at("Z"));

	for (const auto& wpEnemy : Application::Instance().GetGameSystem()->GetEnemyManager()->GetEnemyObjects())
	{
		if (wpEnemy.lock()->GetCollider()->Intersects(Collider::Type::TypeAttack, aabb, nullptr))
		{
			m_bBump = true;
			break;
		}
		else
		{
			m_bBump = false;
		}
	}
}

void Sword::ImGuiUpdate()
{
	ImGui::Text("Bump : %d", m_bBump);
}
