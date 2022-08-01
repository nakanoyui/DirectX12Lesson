#pragma once

class Scene;

class SceneManager
{
private:
	enum class SceneType
	{
		Game,
		Debug,
	};
public:
	void IsChangeScene(const SceneType& sceneType) { m_bChangeScene = true; m_sceneType = sceneType; }

	void Init(const std::shared_ptr<Scene>& spScene);
	void Update();
	void Draw();
	void UpdateLast();

	void ImGuiUpdate();

	const std::shared_ptr<Scene>&GetCurrentScene();

private:

	std::shared_ptr<Scene> m_spCurrentScene = nullptr;

	bool m_bChangeScene;
	SceneType m_sceneType;
};