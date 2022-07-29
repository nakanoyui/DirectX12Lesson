#pragma once

class Scene;

class SceneManager
{
public:
	void AddScene(const std::shared_ptr<Scene>& spScene);

	void ChangeScene(std::string_view sceneName);

	void Init();
	void Update();
	void Draw();

	void ImGuiUpdate();

	std::shared_ptr<Scene> GetCurrentScene();

private:
	std::list< std::shared_ptr<Scene>>m_spScenes;
	std::shared_ptr<Scene>m_spCurrentScene = nullptr;
};