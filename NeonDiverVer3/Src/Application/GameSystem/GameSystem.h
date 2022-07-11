#pragma once

class PlayerManager;
class EnemyManager;
class EnvironmentManager;
class WeaponManager;

class GameSystem
{
public:
	GameSystem() = default;
	~GameSystem() = default;

	void LoadContents();

	void Init();
	void Update();
	void Draw();

	void ImGuiUpdate();

	void AddGameObject(std::shared_ptr<GameObject> spGameObject) { m_spGameObjects.push_back(spGameObject); }

	// アクセサ
	const std::shared_ptr<ShaderManager>GetShaderManager() const { return m_spShaderManager; }
	const std::shared_ptr<JsonManager>GetJsonManager() const { return m_spJsonManager; }
	const std::shared_ptr<CameraManager>GetCameraManager()const { return m_spCameraManager; }

	// オブジェクトマネージャー
	const std::shared_ptr<PlayerManager>GetPlayerManager()const { return m_spPlayerManager; }
	const std::shared_ptr<EnemyManager>GetEnemyManager()const { return m_spEnemyManager; }

private:
	std::shared_ptr<ShaderManager>	m_spShaderManager = nullptr;
	std::shared_ptr<JsonManager>	m_spJsonManager = nullptr;
	std::shared_ptr<CameraManager>	m_spCameraManager = nullptr;

	std::unique_ptr<ThreadManager>	m_upThreadManager = nullptr;

	std::unique_ptr<GameObjectFactory> m_upGameObjectFactory = nullptr;

	std::list<std::shared_ptr<GameObject>>	m_spGameObjects;

	std::shared_ptr<PlayerManager> m_spPlayerManager = nullptr;
	std::shared_ptr<EnemyManager> m_spEnemyManager = nullptr;
	std::shared_ptr<EnvironmentManager> m_spEnvironmentManager = nullptr;
	std::shared_ptr<WeaponManager> m_spWeaponManager = nullptr;

	std::unique_ptr<DrawManager> m_upDrawManager = nullptr;

	bool m_bEnableDebugWireFrame;

	RenderTexture m_fxaa;
	bool m_bEnableFXAA;
};