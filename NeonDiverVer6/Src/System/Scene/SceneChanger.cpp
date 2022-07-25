#include "SceneChanger.h"

#include "Scene.h"

void SceneChanger::AddScene(const std::shared_ptr<Scene>& spScene)
{
	m_spScenes.push_back(spScene);
}

void SceneChanger::ChangeScene(std::string_view sceneName)
{
	for (auto itr = m_spScenes.begin(); itr != m_spScenes.end(); ++itr)
	{
		if ((*itr)->GetSceneName() == sceneName)
		{
			m_spBeforeScene.swap(m_spCurrentScene);
			m_spCurrentScene = (*itr);
			m_spCurrentScene->Init();
			break;
		}
	}
}

const std::shared_ptr<Scene>& SceneChanger::GetCurrentScene()
{
	return m_spCurrentScene;
}

const std::shared_ptr<Scene>& SceneChanger::GetBeforeScene()
{
	return m_spBeforeScene;
}

const std::shared_ptr<Scene>& SceneChanger::GetScene(const std::string_view sceneName)
{
	for (auto itr = m_spScenes.begin(); itr != m_spScenes.end(); ++itr)
	{
		if ((*itr)->GetSceneName() == sceneName)
		{
			return (*itr);
		}
	}

	return nullptr;
}

void SceneChanger::Init()
{
	if (m_spCurrentScene != nullptr)
	{
		m_spCurrentScene->Init();
	}
}

void SceneChanger::Update()
{
	if (m_spCurrentScene != nullptr)
	{
		m_spCurrentScene->Update();
	}
}

void SceneChanger::Draw()
{
	if (m_spCurrentScene != nullptr)
	{
		m_spCurrentScene->Draw();
	}
}

void SceneChanger::ImGuiUpdate()
{
	if (m_spCurrentScene != nullptr)
	{
		m_spCurrentScene->ImGuiUpdate();
	}
}