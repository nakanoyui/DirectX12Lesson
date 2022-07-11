#include "GameSystem.h"

// ObjectManager
#include "GameObject/ModelObject/CharacterObject/PlayerObject/PlayerManager.h"
#include "GameObject/ModelObject/CharacterObject/EnemyObject/EnemyManager.h"
#include "GameObject/ModelObject/EnvironmentObject/EnvironmentManager.h"
#include "GameObject/ModelObject/WeaponObject/WeaponManager.h"

// Character
#include "GameObject/ModelObject/CharacterObject/PlayerObject/Player.h"
#include "GameObject/ModelObject/CharacterObject/EnemyObject/Robot/Robot.h"

// Environment
#include "GameObject/ModelObject/EnvironmentObject/NeonBuilding/NeonBuilding.h"
#include "GameObject/ModelObject/EnvironmentObject/Ground/Ground.h"

// Weapon
#include "GameObject/ModelObject/WeaponObject/Sword/Sword.h"

void GameSystem::LoadContents()
{
	m_spShaderManager = std::make_shared<ShaderManager>();
	m_spJsonManager = std::make_shared<JsonManager>();

	m_spJsonManager->AddJson("Player");
	m_spJsonManager->AddJson("Robot");
	m_spJsonManager->AddJson("Ground");
	m_spJsonManager->AddJson("NeonBuilding");
	m_spJsonManager->AddJson("Sword");

	// シェーダー
	auto spSpriteShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"SpriteShader/SpriteShader");
	spSpriteShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });
	m_spShaderManager->ResisterShader(spSpriteShader,"SpriteShader");

	auto spStandardShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"StandardShader/StandardShader");
	spStandardShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR ,InputLayout::TANGENT},
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spShaderManager->ResisterShader(spStandardShader, "StandardShader");

	auto spSkinMeshShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"SkinMeshShader/SkinMeshShader");
	spSkinMeshShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT },
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spShaderManager->ResisterShader(spSkinMeshShader, "SkinMeshShader");
}

void GameSystem::Init()
{
	m_spCameraManager = std::make_shared<CameraManager>();

	m_upThreadManager = std::make_unique<ThreadManager>();

	m_upDrawManager = std::make_unique<DrawManager>();
	m_upDrawManager->Create("SpriteShader");
	m_upDrawManager->Create("StandardShader");
	m_upDrawManager->Create("SkinMeshShader");

	m_upGameObjectFactory = std::make_unique<GameObjectFactory>();

	m_spPlayerManager = std::make_shared<PlayerManager>();
	m_spEnemyManager = std::make_shared<EnemyManager>();
	m_spEnvironmentManager = std::make_shared<EnvironmentManager>();
	m_spWeaponManager = std::make_shared<WeaponManager>();

	m_upThreadManager->RunThread<Player,PlayerManager,DrawList>(m_upGameObjectFactory, m_spPlayerManager, m_upDrawManager->GetDrawList("SkinMeshShader"));

	m_upThreadManager->RunThread<Robot, EnemyManager, DrawList>(m_upGameObjectFactory, m_spEnemyManager, m_upDrawManager->GetDrawList("StandardShader"));

	m_upThreadManager->RunThread<NeonBuilding, EnvironmentManager, DrawList>(m_upGameObjectFactory, m_spEnvironmentManager, m_upDrawManager->GetDrawList("StandardShader"));

	m_upThreadManager->RunThread<Ground, EnvironmentManager, DrawList>(m_upGameObjectFactory, m_spEnvironmentManager, m_upDrawManager->GetDrawList("StandardShader"));

	m_upThreadManager->RunThread<Sword, WeaponManager, DrawList>(m_upGameObjectFactory, m_spWeaponManager, m_upDrawManager->GetDrawList("StandardShader"));

	m_upThreadManager->WaitForSynchronization();
}

void GameSystem::Update()
{
	for (auto& spGameObject : m_spGameObjects)
	{
		spGameObject->Update();
	}

	m_spCameraManager->UpdateMatrix();
}

void GameSystem::Draw()
{
	GraphicsDevice::Instance().GetCBufferAllocater()->ResetCurrentUseNumber();
	GraphicsDevice::Instance().GetCBVSRVUAVHeap()->Set();

	m_spShaderManager->GetShader("StandardShader")->Begin();

	auto spNowCamera = m_spCameraManager->GetNowCamera();

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach
	(0, ConstantBuffer::cbCamera{ spNowCamera->GetViewMatrix(),spNowCamera->GetProjMatrix(),spNowCamera->GetCameraMatrix().Translation(),spNowCamera->GetViewMatrix().Invert()});

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach(1, ConstantBuffer::cbLight());

	for (auto& spGameObject : m_upDrawManager->GetDrawList("StandardShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawStandard();
	}

	m_spShaderManager->GetShader("SkinMeshShader")->Begin();

	for (auto& spGameObject : m_upDrawManager->GetDrawList("SkinMeshShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawSkinMesh();
	}

	m_spShaderManager->GetShader("SpriteShader")->Begin();

	Math::Matrix mProj2D = DirectX::XMMatrixOrthographicLH(Constant::ScreenSize.x, Constant::ScreenSize.y, 0, 1);

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach
	(1, ConstantBuffer::cbCamera{ spNowCamera->GetViewMatrix(),mProj2D,spNowCamera->GetCameraMatrix().Translation(),spNowCamera->GetViewMatrix().Invert() });

	for (auto& spGameObject : m_upDrawManager->GetDrawList("SpriteShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawSprite();
	}
}

void GameSystem::ImGuiUpdate()
{
	for (auto& spGameObject : m_spGameObjects)
	{
		spGameObject->ImGuiUpdate();
	}
}