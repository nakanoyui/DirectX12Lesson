#include "DebugScene.h"

#include "Application/Application.h"

#include "System/Component/ComponentFactory.h"

void DebugScene::LoadContents()
{
	LoadParameter();

	m_spAssetManager = std::make_shared<AssetManager>();
	m_spJsonManager = std::make_shared<JsonManager>();

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
	m_screen.Create(&GraphicsDevice::Instance(), Constant::ScreenSize / Constant::Two_F, WhiteColor, true);

	m_wpParticleManager = GraphicsDevice::Instance().GetGPUParticleManager();
}

void DebugScene::Init()
{
	m_upDrawManager = std::make_unique<DrawManager>();
	m_upDrawManager->Create("SpriteShader");
	m_upDrawManager->Create("StandardShader");
	m_upDrawManager->Create("SkinMeshShader");
	m_upDrawManager->Create("PlayerShader");
	m_upDrawManager->Create("PointShader");

	m_spComputePipeline = std::make_shared<ComputePipeline>(&GraphicsDevice::Instance(), L"ComputeShader/Particle");
	m_spComputePipeline->Create({ RangeType::UAV,RangeType::SRV });

	//m_upGameObjectFactory->CreateGameObjectModel<Player, PlayerManager, DrawList>(m_spPlayerManager, m_upDrawManager->GetDrawList("PlayerShader"));

	//m_upGameObjectFactory->CreateGameObjectModel<Robot, EnemyManager, DrawList>(m_spEnemyManager, m_upDrawManager->GetDrawList("SkinMeshShader"));

	//for (int i = Constant::Zero_I; i < m_upCSVManager->GetStageData().size(); ++i)
	//{
	//	auto stageData = m_upCSVManager->GetStageData()[i];
	//	switch (stageData.m_mapKind)
	//	{
	//	case static_cast<int>(StageType::Road):
	//		stageData.m_rotate = Constant::Zero_F;
	//		m_upGameObjectFactory->CreateGameObjectModelFromCSV<Ground, EnvironmentManager, DrawList>(m_spEnvironmentManager, m_upDrawManager->GetDrawList("StandardShader"), stageData);
	//		break;
	//	case static_cast<int>(StageType::NeonBuildNotRotate):
	//		stageData.m_rotate = Constant::Zero_F;
	//		m_upGameObjectFactory->CreateGameObjectModelFromCSV<NeonBuilding, EnvironmentManager, DrawList>(m_spEnvironmentManager, m_upDrawManager->GetDrawList("StandardShader"), stageData);
	//		break;
	//	case static_cast<int>(StageType::NeonBuildRotate):
	//		stageData.m_rotate = static_cast<float>(M_PI_2) * ToDegrees;
	//		m_upGameObjectFactory->CreateGameObjectModelFromCSV<NeonBuilding, EnvironmentManager, DrawList>(m_spEnvironmentManager, m_upDrawManager->GetDrawList("StandardShader"), stageData);
	//		break;
	//	default:
	//		break;
	//	}
	//}

	CameraInfo camInfo;
	camInfo.localPos = Math::Vector3(Math::Vector3(0, 5, 0));

	Application::Instance().GetCameraManager()->CreateCamera(camInfo, "PlayerCamera");
	Application::Instance().GetCameraManager()->SetCamera(Application::Instance().GetCameraManager()->GetCamera("PlayerCamera"));
	Application::Instance().GetCameraManager()->SetMatrix(Math::Matrix::Identity);

	m_spComponentFactory = std::make_shared<ComponentFactory>();
	m_spComponentFactory->Init();

	for (const auto& json : m_spJsonManager->GetJsons())
	{
		auto& spGameObject = std::make_shared<GameObject>(json.second->GetConstJson().at("Name"), json.second->GetConstJson().at("Tag"));
		AddGameObject(spGameObject);
	}

	for (const auto& spGameObject : m_spGameObjects)
	{
		spGameObject->Init();
	}

	//m_upGameObjectFactory->CreateGameObjectModel<Sword, WeaponManager, DrawList>(m_spWeaponManager, m_upDrawManager->GetDrawList("StandardShader"));

	//m_upGameObjectFactory->CreateGameObjectParticle<Laser, DrawList>(m_upDrawManager->GetDrawList("PointShader"));

	m_bEnableDebugWireFrame = false;
	m_bEnableFXAA = false;
}

void DebugScene::Update()
{
	//m_wpParticleManager.lock()->Update();

	for (const auto& spGameObject : m_spGameObjects)
	{
		spGameObject->Update();
	}

	CheckDeleteObject();
}

void DebugScene::Draw()
{
	GraphicsDevice::Instance().GetCBufferAllocater()->ResetCurrentUseNumber();
	GraphicsDevice::Instance().GetCBVSRVUAVHeap()->Set();

	GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true, &m_screen);

	//if (m_bEnableFXAA)
	//{
	//	GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true, &m_fxaa);
	//}
	//else
	//{
	//	//GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true);
	//}

	m_spAssetManager->GetShader("StandardShader")->Begin();

	Application::Instance().GetCameraManager()->Set3D(0);

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach(1, ConstantBuffer::cbLight());

	for (const auto& spGameObject : m_spGameObjects)
	{
		spGameObject->Draw();
	}

	//m_spAssetManager->GetShader("SkinMeshShader")->Begin();

	//for (const auto& spGameObject : m_upDrawManager->GetDrawList("SkinMeshShader")->GetGameObjects())
	//{
	//	spGameObject.lock()->Draw();
	//}

	//m_spAssetManager->GetShader("PlayerShader")->Begin();

	//for (const auto& spGameObject : m_upDrawManager->GetDrawList("PlayerShader")->GetGameObjects())
	//{
	//	spGameObject.lock()->Draw();
	//}

	//if (m_bEnableDebugWireFrame)
	//{
	//	m_spAssetManager->GetShader("EffectShader")->Begin();

	//	for (const auto& spGameObject : m_spGameObjects)
	//	{
	//		spGameObject->DrawLate();
	//	}
	//}

	//m_spAssetManager->GetShader("PointShader")->Begin();

	//for (const auto& spGameObject : m_upDrawManager->GetDrawList("PointShader")->GetGameObjects())
	//{
	//	spGameObject.lock()->Draw();
	//}

	m_spAssetManager->GetShader("SpriteShader")->Begin();

	Application::Instance().GetCameraManager()->Set2D(0);

	//for (const auto& spGameObject : m_upDrawManager->GetDrawList("SpriteShader")->GetGameObjects())
	//{
	//	spGameObject.lock()->Draw();
	//}

	//if (m_bEnableFXAA)
	//{
	//	GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true);

	//	m_spAssetManager->GetShader("FXAA")->Begin();
	//	m_spAssetManager->GetShader("FXAA")->DrawTex(&m_fxaa);
	//}

	GraphicsDevice::Instance().ClearRenderTarget(BlueColor, true);

	m_mScreen = Math::Matrix::CreateTranslation(
		m_spJsonManager->GetGeneralJson().at("ScreenPos").at("X"),
		m_spJsonManager->GetGeneralJson().at("ScreenPos").at("Y"),
		m_spJsonManager->GetGeneralJson().at("ScreenPos").at("Z"));

	m_spAssetManager->GetShader("SpriteShader")->Begin();
	m_spAssetManager->GetShader("SpriteShader")->DrawTex(&m_screen, m_mScreen);
}

void DebugScene::ImGuiUpdate()
{
	ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		if (ImGui::Button("ReLoadJson"))
		{
			LoadParameter();
		}

		//if (ImGui::Button("ResetEnemy"))
		//{
		//	for (const auto& wpEnemy : m_spEnemyManager->GetEnemyObjects())
		//	{
		//		wpEnemy.lock()->Reset();
		//	}
		//}

		ImGui::Checkbox("FXAA", &m_bEnableFXAA);

		ImGui::Checkbox("DebugWireFrame", &m_bEnableDebugWireFrame);
	}
	ImGui::End();

	ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		for (const auto& spGameObject : m_spGameObjects)
		{
			spGameObject->ImGuiUpdateHierarchy();
		}
	}
	ImGui::End();

	ImGui::Begin("Inspecter", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		for (const auto& spGameObject : m_spGameObjects)
		{
			spGameObject->ImGuiUpdateInspecter();
		}
	}
	ImGui::End();
}

void DebugScene::LoadParameter()
{
	m_spJsonManager = nullptr;

	m_spJsonManager = std::make_shared<JsonManager>();

	ReferenceJson();
}

void DebugScene::ReferenceJson()
{
	//for (const auto& spGameObject : m_spGameObjects)
	//{
	//	spGameObject->ReferenceJson();
	//}
}

void DebugScene::CheckDeleteObject()
{
	//for (auto it = m_spGameObjects.begin(); it != m_spGameObjects.end();)
	//{
	//	if ((*it)->IsDelete())
	//	{
	//		it = m_spGameObjects.erase(it);
	//		continue;
	//	}

	//	it++;
	//}
}