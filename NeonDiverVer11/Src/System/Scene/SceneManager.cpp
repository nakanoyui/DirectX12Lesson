#include "SceneManager.h"

#include "Application/Application.h"

#include "Scene.h"

#include "Application/Scene/DebugScene/DebugScene.h"

void SceneManager::UpdateLast()
{
	m_spCurrentScene->UpdateLast();

	if (m_bChangeScene)
	{
		m_spCurrentScene = nullptr;

		switch (m_sceneType)
		{
		case SceneManager::SceneType::Game:
		{
			//auto spGameScene = std::make_shared<GameScene>();
			//m_spCurrentScene = spGameScene;
			//m_spCurrentScene->LoadContents();
			//m_spCurrentScene->Init();
		}
		break;
		case SceneManager::SceneType::Debug:
		{
			auto spDebugScene = std::make_shared<DebugScene>();
			m_spCurrentScene = spDebugScene;
			m_spCurrentScene->LoadContents();
			m_spCurrentScene->Init();
		}
		break;
		default:
			break;
		}

		m_bChangeScene = false;
	}
}

void SceneManager::Init(const std::shared_ptr<Scene>& spScene)
{
	m_spCurrentScene = spScene;
	m_spCurrentScene->LoadContents();
	m_spCurrentScene->Init();
}

void SceneManager::Update()
{
	m_spCurrentScene->Update();
}

void SceneManager::Draw()
{
	m_spCurrentScene->Draw();
}

void SceneManager::ImGuiUpdate()
{
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		ImGui::Text("fps : %d", Application::Instance().m_fpsController.m_nowfps);

		//if (ImGui::Button("GameScene"))
		//{
		//	IsChangeScene(SceneType::Game);
		//}

		if (ImGui::Button("DebugScene"))
		{
			IsChangeScene(SceneType::Debug);
		}
	}
	ImGui::End();

	m_spCurrentScene->ImGuiUpdate();
}

const std::shared_ptr<Scene>& SceneManager::GetCurrentScene()
{
	return m_spCurrentScene;
}
