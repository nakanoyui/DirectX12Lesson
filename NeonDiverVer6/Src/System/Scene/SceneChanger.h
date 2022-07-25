#pragma once

class Scene;

class SceneChanger
{
public:
	SceneChanger() = default;
	~SceneChanger() = default;

	void AddScene(const std::shared_ptr<Scene>& spScene);

	void ChangeScene(std::string_view sceneName);

	const std::shared_ptr<Scene>&GetCurrentScene();
	const std::shared_ptr<Scene>&GetBeforeScene();

	const std::shared_ptr<Scene>& GetScene(const std::string_view sceneName);

	void Init();
	void Update();
	void Draw();

	void ImGuiUpdate();

private:
	std::vector<std::shared_ptr<Scene>> m_spScenes;
	std::shared_ptr<Scene> m_spCurrentScene = nullptr;
	std::shared_ptr<Scene> m_spBeforeScene = nullptr;
};