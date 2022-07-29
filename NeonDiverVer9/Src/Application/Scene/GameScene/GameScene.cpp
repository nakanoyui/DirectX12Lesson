#include "GameScene.h"

#include "System/GameObject/GameObjectFactory.h"

// Character
#include "GameObject/ModelObject/CharacterObject/PlayerObject/Player.h"
#include "GameObject/ModelObject/CharacterObject/EnemyObject/Robot/Robot.h"

// Environment
#include "GameObject/ModelObject/EnvironmentObject/NeonBuilding/NeonBuilding.h"
#include "GameObject/ModelObject/EnvironmentObject/Ground/Ground.h"

// Weapon
#include "GameObject/ModelObject/WeaponObject/Sword/Sword.h"

// Particle
#include "GameObject/ParticleObject/Laser/Laser.h"

void GameScene::LoadContents()
{
	LoadParameter();

	m_spAssetManager = std::make_shared<AssetManager>();
	m_spJsonManager = std::make_shared<JsonManager>();

	m_spJsonManager->AddJson("StageData");
	m_spJsonManager->AddJson("Player");
	m_spJsonManager->AddJson("Robot");
	m_spJsonManager->AddJson("Ground");
	m_spJsonManager->AddJson("NeonBuilding");
	m_spJsonManager->AddJson("Sword");
	m_spJsonManager->AddJson("Laser");

	m_upCSVManager = std::make_unique<CSVManager>(m_spJsonManager);
	m_upCSVManager->Load(m_spJsonManager->GetAssetJson().at("CSV").at("StageData"));

	ReferenceJson();

	auto spSpriteShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"SpriteShader/SpriteShader");
	spSpriteShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });
	m_spAssetManager->AddShader(spSpriteShader, "SpriteShader");

	auto spStandardShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"StandardShader/StandardShader");
	spStandardShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR ,InputLayout::TANGENT },
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spAssetManager->AddShader(spStandardShader, "StandardShader");

	auto spSkinMeshShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"SkinMeshShader/SkinMeshShader");
	spSkinMeshShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT },
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spAssetManager->AddShader(spSkinMeshShader, "SkinMeshShader");

	auto spPlayerShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"PlayerShader/PlayerShader");
	spPlayerShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT },
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spAssetManager->AddShader(spPlayerShader, "PlayerShader");

	auto spEffectShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"EffectShader/EffectShader");
	spEffectShader->Create({ InputLayout::POSITION,InputLayout::COLOR }, { RangeType::CBV,RangeType::CBV,RangeType::CBV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
	m_spAssetManager->AddShader(spEffectShader, "EffectShader");

	auto spPointShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"PointShader/PointShader");
	spPointShader->Create({ InputLayout::POSITION }, { RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ADD, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, true, false, 1, true);
	m_spAssetManager->AddShader(spPointShader, "PointShader");

	auto spFXAA = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"FXAA/FXAA");
	spFXAA->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });
	m_spAssetManager->AddShader(spFXAA, "FXAA");

	m_spAssetManager->GetTexture(m_spJsonManager->GetAssetJson().at("Texture").at("Dissolve"));
	m_spAssetManager->GetTexture(m_spJsonManager->GetAssetJson().at("Texture").at("Particle").at("Default"));
	m_spAssetManager->GetTexture(m_spJsonManager->GetAssetJson().at("Texture").at("Particle").at("Laser"));

	MeshData<Vertex>vertex;
	vertex.GetVertices()->push_back({});
	vertex.GetIndices()->push_back(0);
	std::shared_ptr<Mesh>spMesh = std::make_shared<Mesh>();
	spMesh->Create(&GraphicsDevice::Instance(), vertex);
	m_spAssetManager->AddMesh(spMesh, "Point");

	MeshData<TextureVertex>	texVert;
	MeshCreater::Create2DQuad(texVert, Constant::ScreenSize);
	spMesh = nullptr;
	spMesh = std::make_shared<Mesh>();
	spMesh->Create(&GraphicsDevice::Instance(), texVert);
	m_spAssetManager->AddMesh(spMesh, "2DQuad");

	m_fxaa.Create(&GraphicsDevice::Instance(), Constant::ScreenSize, WhiteColor);

	m_wpParticleManager = GraphicsDevice::Instance().GetGPUParticleManager();
}

void GameScene::Init()
{
	m_spPlayerManager = std::make_shared<PlayerManager>();
	m_spEnemyManager = std::make_shared<EnemyManager>();
	m_spEnvironmentManager = std::make_shared<EnvironmentManager>();
	m_spWeaponManager = std::make_shared<WeaponManager>();

	m_upDrawManager = std::make_unique<DrawManager>();
	m_upDrawManager->Create("SpriteShader");
	m_upDrawManager->Create("StandardShader");
	m_upDrawManager->Create("SkinMeshShader");
	m_upDrawManager->Create("PlayerShader");
	m_upDrawManager->Create("PointShader");

	m_spComputePipeline = std::make_shared<ComputePipeline>(&GraphicsDevice::Instance(), L"ComputeShader/Particle");
	m_spComputePipeline->Create({ RangeType::UAV,RangeType::SRV });

	m_upGameObjectFactory = std::make_unique<GameObjectFactory>();

	m_upGameObjectFactory->CreateGameObjectModel<Player, PlayerManager, DrawList>(m_spPlayerManager, m_upDrawManager->GetDrawList("PlayerShader"));

	m_upGameObjectFactory->CreateGameObjectModel<Robot, EnemyManager, DrawList>(m_spEnemyManager, m_upDrawManager->GetDrawList("SkinMeshShader"));

	m_upGameObjectFactory->CreateGameObjectModel<Robot, EnemyManager, DrawList>(m_spEnemyManager, m_upDrawManager->GetDrawList("SkinMeshShader"));

	m_upGameObjectFactory->CreateGameObjectModel<Robot, EnemyManager, DrawList>(m_spEnemyManager, m_upDrawManager->GetDrawList("SkinMeshShader"));

	for (int i = Constant::Zero_I; i < m_upCSVManager->GetStageData().size(); ++i)
	{
		auto stageData = m_upCSVManager->GetStageData()[i];
		switch (stageData.m_mapKind)
		{
		case static_cast<int>(StageType::Road):
			stageData.m_rotate = Constant::Zero_F;
			m_upGameObjectFactory->CreateGameObjectModelFromCSV<Ground, EnvironmentManager, DrawList>(m_spEnvironmentManager, m_upDrawManager->GetDrawList("StandardShader"), stageData);
			break;
		case static_cast<int>(StageType::NeonBuildNotRotate):
			stageData.m_rotate = Constant::Zero_F;
			m_upGameObjectFactory->CreateGameObjectModelFromCSV<NeonBuilding, EnvironmentManager, DrawList>(m_spEnvironmentManager, m_upDrawManager->GetDrawList("StandardShader"), stageData);
			break;
		case static_cast<int>(StageType::NeonBuildRotate):
			stageData.m_rotate = static_cast<float>(M_PI_2) * ToDegrees;
			m_upGameObjectFactory->CreateGameObjectModelFromCSV<NeonBuilding, EnvironmentManager, DrawList>(m_spEnvironmentManager, m_upDrawManager->GetDrawList("StandardShader"), stageData);
			break;
		default:
			break;
		}
	}

	m_upGameObjectFactory->CreateGameObjectModel<Sword, WeaponManager, DrawList>(m_spWeaponManager ,m_upDrawManager->GetDrawList("StandardShader"));

	m_upGameObjectFactory->CreateGameObjectParticle<Laser, DrawList>(m_upDrawManager->GetDrawList("PointShader"));

	m_bEnableDebugWireFrame = false;
	m_bEnableFXAA = false;
}

void GameScene::Update()
{
	m_wpParticleManager.lock()->Update();

	for (auto& spGameObject : m_spGameObjects)
	{
		spGameObject->Update();
	}

	CheckDeleteObject();
}

void GameScene::Draw()
{
	GraphicsDevice::Instance().GetCBufferAllocater()->ResetCurrentUseNumber();
	GraphicsDevice::Instance().GetCBVSRVUAVHeap()->Set();

	if (m_bEnableFXAA)
	{
		GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true, &m_fxaa);
	}
	else
	{
		GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true);
	}

	m_spAssetManager->GetShader("StandardShader")->Begin();

	Application::Instance().GetCameraManager()->Set3D(0);

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach(1, ConstantBuffer::cbLight());

	for (auto& spGameObject : m_upDrawManager->GetDrawList("StandardShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawStandard();
	}

	m_spAssetManager->GetShader("SkinMeshShader")->Begin();

	for (auto& spGameObject : m_upDrawManager->GetDrawList("SkinMeshShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawSkinMesh();
	}

	m_spAssetManager->GetShader("PlayerShader")->Begin();

	for (auto& spGameObject : m_upDrawManager->GetDrawList("PlayerShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawPlayer();
	}

	if (m_bEnableDebugWireFrame)
	{
		m_spAssetManager->GetShader("EffectShader")->Begin();

		for (auto& spGameObject : m_spGameObjects)
		{
			spGameObject->DrawEffect();
		}
	}

	m_spAssetManager->GetShader("PointShader")->Begin();

	for (auto& spGameObject : m_upDrawManager->GetDrawList("PointShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawParticle();
	}

	m_spAssetManager->GetShader("SpriteShader")->Begin();

	Application::Instance().GetCameraManager()->Set2D(0);

	for (auto& spGameObject : m_upDrawManager->GetDrawList("SpriteShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawSprite();
	}

	if (m_bEnableFXAA)
	{
		GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true);

		m_spAssetManager->GetShader("FXAA")->Begin();
		m_spAssetManager->GetShader("FXAA")->DrawTex(&m_fxaa);
	}
}

void GameScene::ImGuiUpdate()
{
	ImGui::Begin("Debug");
	{
		if (ImGui::Button("ReLoadJson"))
		{
			LoadParameter();
		}

		if (ImGui::Button("ResetEnemy"))
		{
			for (auto& wpEnemy : m_spEnemyManager->GetEnemyObjects())
			{
				wpEnemy.lock()->Reset();
			}
		}

		ImGui::Checkbox("FXAA", &m_bEnableFXAA);

		ImGui::Checkbox("DebugWireFrame", &m_bEnableDebugWireFrame);
	}
	ImGui::End();

	ImGui::Begin("GameObject");
	{
		for (auto& spGameObject : m_spGameObjects)
		{
			spGameObject->ImGuiUpdateGameObject();
		}
	}
	ImGui::End();

	ImGui::Begin("ParticleSystem");
	{
		for (auto& spGameObject : m_spGameObjects)
		{
			spGameObject->ImGuiUpdateParticle();
		}
	}
	ImGui::End();
}

void GameScene::LoadParameter()
{
	m_spJsonManager = nullptr;
	m_upCSVManager = nullptr;

	m_spJsonManager = std::make_shared<JsonManager>();
	m_upCSVManager = std::make_unique<CSVManager>(m_spJsonManager);

	m_spJsonManager->AddJson("StageData");
	m_spJsonManager->AddJson("Player");
	m_spJsonManager->AddJson("Robot");
	m_spJsonManager->AddJson("Ground");
	m_spJsonManager->AddJson("NeonBuilding");
	m_spJsonManager->AddJson("Sword");
	m_spJsonManager->AddJson("Laser");

	m_upCSVManager->Load(m_spJsonManager->GetAssetJson().at("CSV").at("StageData"));

	ReferenceJson();
}

void GameScene::ReferenceJson()
{
	for (auto& spGameObject : m_spGameObjects)
	{
		spGameObject->ReferenceJson();
	}
}

void GameScene::CheckDeleteObject()
{
	for (auto it = m_spGameObjects.begin(); it != m_spGameObjects.end();)
	{
		if ((*it)->IsDelete())
		{
			it = m_spGameObjects.erase(it);
			continue;
		}

		it++;
	}
}