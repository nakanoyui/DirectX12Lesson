#pragma once

class SpriteFont;

//
class GameObjectFactory;

//
class PlayerManager;
class EnemyManager;
class EnvironmentManager;
class WeaponManager;

//
class ShaderManager;
class ModelManager;
class TextureManeger;
class JsonManager;
class CameraManager;
class CSVManager;
class DrawManager;

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

	void AddGameObject(const std::shared_ptr<GameObject>& spGameObject) { m_spGameObjects.push_back(spGameObject); }

	// アクセサ
	const std::shared_ptr<ShaderManager>&GetShaderManager() const { return m_spShaderManager; }
	const std::shared_ptr<ModelManager>&GetModelManager() const { return m_spModelManager; }
	const std::shared_ptr<TextureManeger>&GetTextureManeger() const { return m_spTextureManeger; }
	const std::shared_ptr<JsonManager>&GetJsonManager() const { return m_spJsonManager; }
	const std::shared_ptr<CameraManager>&GetCameraManager()const { return m_spCameraManager; }

	// オブジェクトマネージャー
	const std::shared_ptr<PlayerManager>&GetPlayerManager()const { return m_spPlayerManager; }
	const std::shared_ptr<EnemyManager>&GetEnemyManager()const { return m_spEnemyManager; }
	const std::shared_ptr<EnvironmentManager>&GetEnvironmentManager()const { return m_spEnvironmentManager; }

	//
	const float GetWorldGravity()const { return m_worldGravity; }

private:
	void LoadParameter();

	std::unique_ptr<SpriteFont> m_upSpriteFont = nullptr;
	Math::Color m_fontColor;

	std::unique_ptr<GameObjectFactory> m_upGameObjectFactory = nullptr;

	std::list<std::shared_ptr<GameObject>>	m_spGameObjects;

	std::shared_ptr<PlayerManager> m_spPlayerManager = nullptr;
	std::shared_ptr<EnemyManager> m_spEnemyManager = nullptr;
	std::shared_ptr<EnvironmentManager> m_spEnvironmentManager = nullptr;
	std::shared_ptr<WeaponManager> m_spWeaponManager = nullptr;

	std::shared_ptr<ShaderManager>	m_spShaderManager = nullptr;
	std::shared_ptr<ModelManager>	m_spModelManager = nullptr;
	std::shared_ptr<TextureManeger>	m_spTextureManeger = nullptr;
	std::shared_ptr<JsonManager>	m_spJsonManager = nullptr;
	std::shared_ptr<CameraManager>	m_spCameraManager = nullptr;
	std::shared_ptr<CSVManager>		m_spCSVManager = nullptr;

	std::unique_ptr<DrawManager> m_upDrawManager = nullptr;

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