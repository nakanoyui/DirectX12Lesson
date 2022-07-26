//#pragma once
//
////
//class PlayerManager;
//class EnemyManager;
//class EnvironmentManager;
//class WeaponManager;
//
////
//class ShaderManager;
//class ModelManager;
//class TextureManeger;
//class JsonManager;
//class CameraManager;
//class CSVManager;
//class DrawManager;
//
//class Scene
//{
//public:
//	Scene() = default;
//	~Scene() = default;
//
//	virtual void Init() = 0;
//	virtual void Update() = 0;
//	virtual void Draw() = 0;
//
//	virtual void ImGuiUpdate() = 0;
//
//	const std::string& GetSceneName() { return m_sceneName; }
//
//	// オブジェクトマネージャー
//	const std::shared_ptr<PlayerManager>& GetPlayerManager()const { return m_spPlayerManager; }
//	const std::shared_ptr<EnemyManager>& GetEnemyManager()const { return m_spEnemyManager; }
//	const std::shared_ptr<EnvironmentManager>& GetEnvironmentManager()const { return m_spEnvironmentManager; }
//	const std::shared_ptr<WeaponManager>& GetWeaponManager()const { return m_spWeaponManager; }
//
//	void AddGameObject(const std::shared_ptr<GameObject>& spGameObject) { m_spGameObjects.push_back(spGameObject); }
//
//	bool IsChangeScene() { return m_bChangeNextScene; }
//
//protected:
//	std::list<std::shared_ptr<GameObject>>	m_spGameObjects;
//
//	std::weak_ptr<SpriteFont>		m_wpSpriteFont;
//
//	std::weak_ptr<CameraManager>	m_wpCameraManager;
//	std::weak_ptr<ShaderManager>	m_wpShaderManager;
//	std::weak_ptr<TextureManeger>	m_wpTextureManeger;
//	std::weak_ptr<JsonManager>		m_wpJsonManager;
//	std::weak_ptr<CSVManager>		m_wpCSVManager;
//	std::weak_ptr<ModelManager>		m_wpModelManager;
//
//	std::shared_ptr<PlayerManager> m_spPlayerManager = nullptr;
//	std::shared_ptr<EnemyManager> m_spEnemyManager = nullptr;
//	std::shared_ptr<EnvironmentManager> m_spEnvironmentManager = nullptr;
//	std::shared_ptr<WeaponManager> m_spWeaponManager = nullptr;
//
//	std::unique_ptr<DrawManager> m_upDrawManager = nullptr;
//
//	std::string m_sceneName;
//private:
//	bool m_bChangeNextScene;
//};