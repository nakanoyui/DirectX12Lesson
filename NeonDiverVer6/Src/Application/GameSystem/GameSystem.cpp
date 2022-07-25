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
	m_upSpriteFont = std::make_unique<SpriteFont>();
	m_upSpriteFont->Init(&GraphicsDevice::Instance());
	m_fontColor = BlackColor;
	m_fontColor.w = Constant::Zero_F;

	m_spShaderManager = std::make_shared<ShaderManager>();
	m_spModelManager = std::make_shared<ModelManager>();
	m_spTextureManeger = std::make_shared<TextureManeger>();

	LoadParameter();

	// シェーダー
	auto spSpriteShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"SpriteShader/SpriteShader");
	spSpriteShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });
	m_spShaderManager->ResisterShader(spSpriteShader, "SpriteShader");

	auto spStandardShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"StandardShader/StandardShader");
	spStandardShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR ,InputLayout::TANGENT },
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spShaderManager->ResisterShader(spStandardShader, "StandardShader");

	auto spSkinMeshShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"SkinMeshShader/SkinMeshShader");
	spSkinMeshShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT },
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spShaderManager->ResisterShader(spSkinMeshShader, "SkinMeshShader");

	auto spPlayerShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"PlayerShader/PlayerShader");
	spPlayerShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT },
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spShaderManager->ResisterShader(spPlayerShader, "PlayerShader");

	auto spEffectShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"EffectShader/EffectShader");
	spEffectShader->Create({ InputLayout::POSITION,InputLayout::COLOR }, { RangeType::CBV,RangeType::CBV,RangeType::CBV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
	m_spShaderManager->ResisterShader(spEffectShader, "EffectShader");

	auto spFXAA = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"FXAA/FXAA");
	spFXAA->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });
	m_spShaderManager->ResisterShader(spFXAA, "FXAA");

	m_fxaa.Create(&GraphicsDevice::Instance(), Constant::ScreenSize, WhiteColor);

	m_spTextureManeger->ResisterTexture(m_spJsonManager->GetAssetJson().at("Texture").at("Dissolve"));
}

void GameSystem::LoadParameter()
{
	m_spJsonManager = nullptr;
	m_spCSVManager = nullptr;

	m_spJsonManager = std::make_shared<JsonManager>();
	m_spCSVManager = std::make_shared<CSVManager>(m_spJsonManager);

	m_spJsonManager->AddJson("StageData");
	m_spJsonManager->AddJson("Player");
	m_spJsonManager->AddJson("Robot");
	m_spJsonManager->AddJson("Ground");
	m_spJsonManager->AddJson("NeonBuilding");
	m_spJsonManager->AddJson("Sword");

	m_spCSVManager->Load(m_spJsonManager->GetAssetJson().at("CSV").at("StageData"));

	for (auto& spGameObject : m_spGameObjects)
	{
		spGameObject->ReferenceJson();
	}
}

void GameSystem::Init()
{
	m_spCameraManager = std::make_shared<CameraManager>();

	m_upDrawManager = std::make_unique<DrawManager>();
	m_upDrawManager->Create("SpriteShader");
	m_upDrawManager->Create("StandardShader");
	m_upDrawManager->Create("SkinMeshShader");
	m_upDrawManager->Create("PlayerShader");

	m_upGameObjectFactory = std::make_unique<GameObjectFactory>();

	m_spPlayerManager = std::make_shared<PlayerManager>();
	m_spEnemyManager = std::make_shared<EnemyManager>();
	m_spEnvironmentManager = std::make_shared<EnvironmentManager>();
	m_spWeaponManager = std::make_shared<WeaponManager>();

	m_upGameObjectFactory->CreateGameObjectModel<Player, PlayerManager, DrawList>(m_spPlayerManager, m_upDrawManager->GetDrawList("PlayerShader"));

	m_upGameObjectFactory->CreateGameObjectModel<Robot, EnemyManager, DrawList>(m_spEnemyManager, m_upDrawManager->GetDrawList("SkinMeshShader"));

	m_upGameObjectFactory->CreateGameObjectModel<Robot, EnemyManager, DrawList>(m_spEnemyManager, m_upDrawManager->GetDrawList("SkinMeshShader"));

	m_upGameObjectFactory->CreateGameObjectModel<Robot, EnemyManager, DrawList>(m_spEnemyManager, m_upDrawManager->GetDrawList("SkinMeshShader"));

	for (int i = Constant::Zero_I; i < m_spCSVManager->GetStageData().size(); ++i)
	{
		auto stageData = m_spCSVManager->GetStageData()[i];
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
				stageData.m_rotate = DirectX::XMConvertToDegrees(static_cast<float>(M_PI_2));
				m_upGameObjectFactory->CreateGameObjectModelFromCSV<NeonBuilding, EnvironmentManager, DrawList>(m_spEnvironmentManager, m_upDrawManager->GetDrawList("StandardShader"), stageData);
				break;
		default:
			break;
		}
	}

	m_upGameObjectFactory->CreateGameObjectModel<Sword, WeaponManager, DrawList>(m_spWeaponManager, m_upDrawManager->GetDrawList("StandardShader"));

	m_bEnableDebugWireFrame = false;
	m_bEnableFXAA = false;
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

	if (m_bEnableFXAA)
	{
		GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true, &m_fxaa);
	}
	else
	{
		GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true);
	}

	auto spNowCamera = m_spCameraManager->GetNowCamera();

	m_spShaderManager->GetShader("StandardShader")->Begin();

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach
	(0, ConstantBuffer::cbCamera{ spNowCamera->GetViewMatrix(),spNowCamera->GetProjMatrix(),spNowCamera->GetViewMatrix().Invert(),spNowCamera->GetCameraMatrix().Translation() });

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

	m_spShaderManager->GetShader("PlayerShader")->Begin();

	for (auto& spGameObject : m_upDrawManager->GetDrawList("PlayerShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawPlayer();
	}

	if (m_bEnableDebugWireFrame)
	{
		m_spShaderManager->GetShader("EffectShader")->Begin();

		for (auto& spGameObject : m_spGameObjects)
		{
			spGameObject->DrawEffect();
		}
	}

	m_spShaderManager->GetShader("SpriteShader")->Begin();

	Math::Matrix mProj2D = DirectX::XMMatrixOrthographicLH(Constant::ScreenSize.x, Constant::ScreenSize.y, 0, 1);

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach
	(0, ConstantBuffer::cbCamera{ spNowCamera->GetViewMatrix(),mProj2D,spNowCamera->GetViewMatrix().Invert(),spNowCamera->GetCameraMatrix().Translation() });

	for (auto& spGameObject : m_upDrawManager->GetDrawList("SpriteShader")->GetGameObjects())
	{
		spGameObject.lock()->DrawSprite();
	}

	if (m_bEnableFXAA)
	{
		GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true);

		m_spShaderManager->GetShader("FXAA")->Begin();
		m_spShaderManager->GetShader("FXAA")->DrawTex(&m_fxaa);
	}

	if (m_fontColor.w > Constant::Zero_F)
	{
		m_upSpriteFont->Begin();
		m_upSpriteFont->GetSpriteFont()->DrawString(m_upSpriteFont->GetSpriteBatch().get(), L"Jsonファイルの再読み込みを行いました", Math::Vector2(250, Constant::ScreenSize.y / Constant::Two_F), m_fontColor);
		m_upSpriteFont->End();

		m_fontColor.w -= Constant::SpriteFontAttenuation;
	}
}

void GameSystem::ImGuiUpdate()
{
	ImGui::Begin("Debug");
	if (ImGui::Button("ReLoadJson"))
	{
		LoadParameter();
		m_fontColor.w = Constant::One_F;
	}
	ImGui::Checkbox("DebugWireFrame", &m_bEnableDebugWireFrame);
	ImGui::End();

	ImGui::Begin("PostEffect");
	ImGui::Checkbox("FXAA", &m_bEnableFXAA);
	ImGui::End();

	for (auto& spGameObject : m_spGameObjects)
	{
		spGameObject->ImGuiUpdate();
	}

	if (ImGui::Button("ResetEnemy"))
	{
		for (auto& wpEnemy : m_spEnemyManager->GetEnemyObjects())
		{
			wpEnemy.lock()->Reset();
		}
	}
}