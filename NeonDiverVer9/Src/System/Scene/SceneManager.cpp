#include "SceneManager.h"

#include "Scene.h"

void SceneManager::AddScene(const std::shared_ptr<Scene>& spScene)
{
	m_spScenes.emplace_back(spScene);
}

void SceneManager::ChangeScene(std::string_view sceneName)
{
	for (auto it = m_spScenes.begin(); it != m_spScenes.end(); ++it)
	{
		if ((*it)->GetName() == sceneName)
		{
			m_spCurrentScene = nullptr;
			m_spCurrentScene = (*it);
			m_spCurrentScene->LoadContents();
			m_spCurrentScene->Init();
			break;
		}
	}
}

void SceneManager::Init()
{
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
	m_spCurrentScene->ImGuiUpdate();
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return m_spCurrentScene;
}
