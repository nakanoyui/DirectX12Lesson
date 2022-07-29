#pragma once

//
class PlayerManager;
class EnemyManager;
class EnvironmentManager;
class WeaponManager;

class ComputePipeline;

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

	std::string GetName() { return m_name; }

	void AddGameObject(const std::shared_ptr<GameObject>&spGameObject) { m_spGameObjects.push_back(spGameObject); }

	const std::shared_ptr<AssetManager>& GetAssetManager() const { return m_spAssetManager; }
	const std::shared_ptr<JsonManager>& GetJsonManager() const { return m_spJsonManager; }

	const std::shared_ptr<PlayerManager>& GetPlayerManager()const { return m_spPlayerManager; }
	const std::shared_ptr<EnemyManager>& GetEnemyManager()const { return m_spEnemyManager; }
	const std::shared_ptr<EnvironmentManager>& GetEnvironmentManager()const { return m_spEnvironmentManager; }
	const std::shared_ptr<WeaponManager>& GetWeaponManager()const { return m_spWeaponManager; }

	const std::unique_ptr<DrawManager>& GetDrawManager()const { return m_upDrawManager; }

	const std::shared_ptr<ComputePipeline>& GetComputePipeline()const { return m_spComputePipeline; }

	//
	const float GetWorldGravity()const { return m_worldGravity; }

protected:
	std::list<std::shared_ptr<GameObject>>m_spGameObjects;

	std::shared_ptr<AssetManager>	m_spAssetManager = nullptr;
	std::shared_ptr<JsonManager>	m_spJsonManager = nullptr;

	std::shared_ptr<PlayerManager> m_spPlayerManager = nullptr;
	std::shared_ptr<EnemyManager> m_spEnemyManager = nullptr;
	std::shared_ptr<EnvironmentManager> m_spEnvironmentManager = nullptr;
	std::shared_ptr<WeaponManager> m_spWeaponManager = nullptr;

	std::unique_ptr<DrawManager> m_upDrawManager = nullptr;

	std::shared_ptr<ComputePipeline> m_spComputePipeline = nullptr;

	std::string m_name;

	const float m_worldGravity = 0.01f;
};