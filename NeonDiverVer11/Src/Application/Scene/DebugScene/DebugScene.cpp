#include "DebugScene.h"

#include "Application/Application.h"

#include "System/Component/ComponentFactory.h"

void DebugScene::LoadContents()
{
	m_spAssetManager = std::make_shared<AssetManager>();
	m_spJsonManager = std::make_shared<JsonManager>();

	auto spSpriteShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"SpriteShader/SpriteShader");
	spSpriteShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });
	m_spAssetManager->AddSpriteShader(spSpriteShader, "SpriteShader");

	auto spStandardShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"StandardShader/StandardShader");
	spStandardShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR ,InputLayout::TANGENT },
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spAssetManager->AddStandardShader(spStandardShader, "StandardShader");

	auto spSkinMeshShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"SkinMeshShader/SkinMeshShader");
	spSkinMeshShader->Create(
		{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT },
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	m_spAssetManager->AddSkinMeshShaders(spSkinMeshShader, "SkinMeshShader");

	//auto spPlayerShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"PlayerShader/PlayerShader");
	//spPlayerShader->Create(
	//	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT },
	//	{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });
	//m_spAssetManager->AddSkinMeshShaders(spPlayerShader, "PlayerShader");

	auto spEffectShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"EffectShader/EffectShader");
	spEffectShader->Create({ InputLayout::POSITION,InputLayout::COLOR }, { RangeType::CBV,RangeType::CBV,RangeType::CBV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
	m_spAssetManager->AddStandardShader(spEffectShader, "EffectShader");

	//auto spPointShader = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"PointShader/PointShader");
	//spPointShader->Create({ InputLayout::POSITION }, { RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ADD, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, true, false, 1, true);
	//m_spAssetManager->AddStandardShader(spPointShader, "PointShader");

	//auto spFXAA = std::make_shared<Shader>(&GraphicsDevice::Instance(), L"FXAA/FXAA");
	//spFXAA->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });
	//m_spAssetManager->AddSpriteShader(spFXAA, "FXAA");

	m_spAssetManager->GetTexture(m_spJsonManager->GetAssetJson().GetConstJson().at("Texture").at("Dissolve"));
	m_spAssetManager->GetTexture(m_spJsonManager->GetAssetJson().GetConstJson().at("Texture").at("Particle").at("Default"));
	m_spAssetManager->GetTexture(m_spJsonManager->GetAssetJson().GetConstJson().at("Texture").at("Particle").at("Laser"));

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
	m_spDrawManager = std::make_shared<DrawManager>();
	m_spDrawManager->Create("SpriteShader");
	m_spDrawManager->Create("StandardShader");
	m_spDrawManager->Create("SkinMeshShader");
	m_spDrawManager->Create("EffectShader");
	//m_spDrawManager->Create("PlayerShader");
	//m_spDrawManager->Create("PointShader");

	m_spComputePipeline = std::make_shared<ComputePipeline>(&GraphicsDevice::Instance(), L"ComputeShader/Particle");
	m_spComputePipeline->Create({ RangeType::UAV,RangeType::SRV });

	m_spComponentFactory = std::make_shared<ComponentFactory>();
	m_spComponentFactory->Init();

	for (const auto& json : m_spJsonManager->GetJsons())
	{
		auto& spGameObject = std::make_shared<GameObject>(json.second->GetConstJson().at("Name"), json.second->GetConstJson().at("Tag"));
		AddGameObject(spGameObject);
		spGameObject->Init();
	}

	m_bExists = false;
}

void DebugScene::Update()
{
	for (auto& spDrawList : m_spDrawManager->GetDrawLists())
	{
		spDrawList.second->WorkGameObjects().clear();
	}

	//m_wpParticleManager.lock()->Update();

	for (const auto& spGameObject : m_spGameObjects)
	{
		if (spGameObject->GetEnable())
		{
			spGameObject->Update();
		}
	}
}

void DebugScene::Draw()
{
	GraphicsDevice::Instance().GetCBufferAllocater()->ResetCurrentUseNumber();
	GraphicsDevice::Instance().GetCBVSRVUAVHeap()->Set();

	GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true, &m_screen);
	{
		//if (m_bEnableFXAA)
		//{
		//	GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true, &m_fxaa);
		//}
		//else
		//{
		//	//GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true);
		//}

		m_spAssetManager->GetStandardShader("StandardShader")->Begin();

		Application::Instance().GetCameraManager()->Set3D(0);

		GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach(1, ConstantBuffer::cbLight());

		for (const auto& wpGameObject : m_spDrawManager->GetDrawList("StandardShader")->GetGameObjects())
		{
			if (wpGameObject.lock()->GetEnable())
			{
				wpGameObject.lock()->Draw();
			}
		}

		m_spAssetManager->GetSkinMeshShader("SkinMeshShader")->Begin();

		for (const auto& wpGameObject : m_spDrawManager->GetDrawList("SkinMeshShader")->GetGameObjects())
		{
			if (wpGameObject.lock()->GetEnable())
			{
				wpGameObject.lock()->Draw();
			}
		}

		//m_spAssetManager->GetSkinMeshShader("PlayerShader")->Begin();

		//for (const auto& wpGameObject : m_spDrawManager->GetDrawList("PlayerShader")->GetGameObjects())
		//{
		//	if (wpGameObject.lock()->GetEnable())
		//	{
		//		wpGameObject.lock()->Draw();
		//	}
		//}

		//if (m_bEnableDebugWireFrame)
		{
			m_spAssetManager->GetStandardShader("EffectShader")->Begin();

			for (const auto& wpGameObject : m_spDrawManager->GetDrawList("EffectShader")->GetGameObjects())
			{
				if (wpGameObject.lock()->GetEnable())
				{
					wpGameObject.lock()->DrawLate();
				}
			}
		}

		//m_spAssetManager->GetStandardShader("PointShader")->Begin();

		//for (const auto& wpGameObject : m_spDrawManager->GetDrawList("PointShader")->GetGameObjects())
		//{
		//	if (wpGameObject.lock()->GetEnable())
		//	{
		//		wpGameObject.lock()->Draw();
		//	}
		//}

		m_spAssetManager->GetSpriteShader("SpriteShader")->Begin();

		Application::Instance().GetCameraManager()->Set2D(0);

		for (const auto& wpGameObject : m_spDrawManager->GetDrawList("SpriteShader")->GetGameObjects())
		{
			if (wpGameObject.lock()->GetEnable())
			{
				wpGameObject.lock()->Draw();
			}
		}

		//if (m_bEnableFXAA)
		//{
		//	GraphicsDevice::Instance().ClearRenderTarget(WhiteColor, true);

		//	m_spAssetManager->GetShader("FXAA")->Begin();
		//	m_spAssetManager->GetShader("FXAA")->DrawTex(&m_fxaa);
		//}
	}

	GraphicsDevice::Instance().ClearRenderTarget(BlackColor, true);
	{
		m_mScreen = Math::Matrix::CreateTranslation(
			m_spJsonManager->GetGeneralJson().GetConstJson().at("ScreenPos").at("X"),
			m_spJsonManager->GetGeneralJson().GetConstJson().at("ScreenPos").at("Y"),
			m_spJsonManager->GetGeneralJson().GetConstJson().at("ScreenPos").at("Z"));

		m_spAssetManager->GetSpriteShader("SpriteShader")->Begin();
		m_spAssetManager->GetSpriteShader("SpriteShader")->DrawTex(&m_screen, m_mScreen);
	}
}

void DebugScene::ImGuiUpdate()
{
	ImGui::Begin("GameObjectFactory", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		if (ImGui::TreeNode("EmptyObject"))
		{
			ImGui::InputText("Name", m_names, ImGuiHelper::InputTextSize);

			ImGui::InputText("Tag", m_tags, ImGuiHelper::InputTextSize);

			if (ImGui::Button("Create"))
			{
				m_bExists = false;

				for (const auto& spGameObject : m_spGameObjects)
				{
					if (spGameObject->GetName() == m_names)
					{
						m_bExists = true;
					}
				}

				if (!m_bExists)
				{
					auto& spGameObject = std::make_shared<GameObject>(m_names, m_tags);
					AddGameObject(spGameObject);
					spGameObject->Create();
					spGameObject->AddComponent("TransformComponent", spGameObject->GetName());
					spGameObject->BakeJson();
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("3DObject"))
		{
			ImGui::InputText("Name", m_names, ImGuiHelper::InputTextSize);

			ImGui::InputText("Tag", m_tags, ImGuiHelper::InputTextSize);

			if (ImGui::Button("Create"))
			{
				m_bExists = false;

				for (const auto& spGameObject : m_spGameObjects)
				{
					if (spGameObject->GetName() == m_names)
					{
						m_bExists = true;
					}
				}

				if (!m_bExists)
				{
					auto& spGameObject = std::make_shared<GameObject>(m_names, m_tags);
					AddGameObject(spGameObject);
					spGameObject->Create();
					spGameObject->AddComponent("TransformComponent", spGameObject->GetName());
					spGameObject->AddComponent("ModelComponent", spGameObject->GetName());
					spGameObject->AddComponent("StandardRendererComponent", spGameObject->GetName());
					spGameObject->BakeJson();
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Camera"))
		{
			ImGui::InputText("Name", m_names, ImGuiHelper::InputTextSize);

			ImGui::InputText("Tag", m_tags, ImGuiHelper::InputTextSize);

			if (ImGui::Button("Create"))
			{
				m_bExists = false;

				for (const auto& spGameObject : m_spGameObjects)
				{
					if (spGameObject->GetName() == m_names)
					{
						m_bExists = true;
					}
				}

				if (!m_bExists)
				{
					auto& spGameObject = std::make_shared<GameObject>(m_names, m_tags);
					AddGameObject(spGameObject);
					spGameObject->Create();
					spGameObject->AddComponent("TransformComponent", spGameObject->GetName());
					spGameObject->AddComponent("CameraComponent", spGameObject->GetName());
					spGameObject->BakeJson();
				}
			}

			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("RemoveGameObject"))
		{
			m_pExistsGameObject.clear();

			for (const auto& spGameObject : m_spGameObjects)
			{
				m_pExistsGameObject.emplace_back(const_cast<char*>(spGameObject->GetName().c_str()));
			}

			int i;
			if (ImGui::ListBox("", &i, m_pExistsGameObject.data(), static_cast<int>(m_pExistsGameObject.size())))
			{
				RemoveGameObject(m_pExistsGameObject[i]);
			}

			ImGui::TreePop();
		}

		if (m_bExists)
		{
			ImGui::Separator();
			ImGui::TextColored(ImVec4(0xff, 0x00, 0x00, 0xff), "You can't create game objects with the same name");
		}
	}
	ImGui::End();

	ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		for (const auto& spGameObject : m_spGameObjects)
		{
			spGameObject->ImGuiUpdateHierarchy();
		}
	}
	ImGui::End();

	ImGui::Begin("Inspecter", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		for (const auto& spGameObject : m_spGameObjects)
		{
			spGameObject->ImGuiUpdateInspecter();
		}
	}
	ImGui::End();
}

void DebugScene::UpdateLast()
{
	for (const auto& spGameObject : m_spGameObjects)
	{
		spGameObject->CheckDeleteComponent();
	}
}