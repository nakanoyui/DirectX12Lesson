#pragma once

//
class GameObjectFactory;

//
class PlayerManager;
class EnemyManager;
class EnvironmentManager;
class WeaponManager;

class JsonManager;
class CameraManager;
class CSVManager;
class DrawManager;

class GameManager
{
public:
	enum class GameState
	{
		None,
		Title,
		Game,
		Result,
		Paused,
		Quit,
		Max,
	};

	GameManager();
	~GameManager();

	void LoadContents();
	void Init();
	void Update();
	void Draw();

	void ImGuiUpdate();

	void AddGameObject(const std::shared_ptr<GameObject>& spGameObject);
	void CheckDeleteGameObject();

	// アクセサ
	const std::shared_ptr<AssetManager>& GetAssetManager() const { return m_spAssetManager; }
	const std::shared_ptr<JsonManager>& GetJsonManager() const { return m_spJsonManager; }
	const std::shared_ptr<CameraManager>& GetCameraManager()const { return m_spCameraManager; }

	const std::shared_ptr<ComputePipeline>& GetComputePipeline()const { return m_spComputePipeline; }

	// オブジェクトマネージャー
	const std::shared_ptr<PlayerManager>& GetPlayerManager()const { return m_spPlayerManager; }
	const std::shared_ptr<EnemyManager>& GetEnemyManager()const { return m_spEnemyManager; }
	const std::shared_ptr<EnvironmentManager>& GetEnvironmentManager()const { return m_spEnvironmentManager; }
	const std::shared_ptr<WeaponManager>& GetWeaponManager()const { return m_spWeaponManager; }

	//
	const float GetWorldGravity()const { return m_worldGravity; }

private:
	void Release();

	void LoadParameter();

	GameState m_gameState;

	std::unique_ptr<GameObjectFactory> m_upGameObjectFactory = nullptr;

	std::list<std::shared_ptr<GameObject>> m_spGameObjects;

	std::shared_ptr<PlayerManager> m_spPlayerManager = nullptr;
	std::shared_ptr<EnemyManager> m_spEnemyManager = nullptr;
	std::shared_ptr<EnvironmentManager> m_spEnvironmentManager = nullptr;
	std::shared_ptr<WeaponManager> m_spWeaponManager = nullptr;

	std::shared_ptr<AssetManager>	m_spAssetManager = nullptr;
	std::shared_ptr<JsonManager>	m_spJsonManager = nullptr;
	std::shared_ptr<CSVManager>		m_spCSVManager = nullptr;

	std::shared_ptr<CameraManager>	m_spCameraManager = nullptr;

	std::unique_ptr<DrawManager> m_upDrawManager = nullptr;

	std::shared_ptr<ComputePipeline> m_spComputePipeline = nullptr;
	std::weak_ptr<GPUParticleManager> m_wpParticleManager;

	bool m_bEnableDebugWireFrame;

	// FXAA
	RenderTexture m_fxaa;
	bool m_bEnableFXAA;

	enum class StageType
	{
		None,
		Road,
		NeonBuildNotRotate,
		NeonBuildRotate,
	};

	const float m_worldGravity = 0.01f;
};