#pragma once

class ComputePipeline;
class ComponentFactory;

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void LoadContents() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void ImGuiUpdate() = 0;

	void AddGameObject(const std::shared_ptr<GameObject>&spGameObject) { m_spGameObjects.push_back(spGameObject); }

	const std::list<std::shared_ptr<GameObject>>& GetGameObjects() { return m_spGameObjects; }

	const std::shared_ptr<ComponentFactory>&GetComponentFactory()const { return m_spComponentFactory; }

	const std::shared_ptr<AssetManager>& GetAssetManager() const { return m_spAssetManager; }
	const std::shared_ptr<JsonManager>& GetJsonManager() const { return m_spJsonManager; }

	const std::unique_ptr<DrawManager>& GetDrawManager()const { return m_upDrawManager; }

	const std::shared_ptr<ComputePipeline>& GetComputePipeline()const { return m_spComputePipeline; }

	//
	const float GetWorldGravity()const { return m_worldGravity; }

protected:
	std::list<std::shared_ptr<GameObject>>m_spGameObjects;

	std::shared_ptr<ComponentFactory> m_spComponentFactory = nullptr;

	std::shared_ptr<AssetManager>	m_spAssetManager = nullptr;
	std::shared_ptr<JsonManager>	m_spJsonManager = nullptr;

	std::unique_ptr<DrawManager> m_upDrawManager = nullptr;

	std::shared_ptr<ComputePipeline> m_spComputePipeline = nullptr;

	const float m_worldGravity = 0.01f;
};