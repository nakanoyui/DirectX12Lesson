#pragma once

class SceneManager;
class KeyManager;

struct FPSController
{
	// FPS制御
	int		m_nowfps = 0;		// 現在のFPS値
	int		m_maxFps = 60;		// 最大FPS

	void Init();

	void UpdateStart();

	void Update();

private:

	LARGE_INTEGER Freq = { 0 };
	LARGE_INTEGER StartTime = { 0 };
	LARGE_INTEGER NowTime = { 0 };
	int iCount = 0;
	DWORD SleepTime = 0;
};

class Application
{
public:
	~Application() = default;

	void Excute();

	const std::shared_ptr<KeyManager>& GetKeyManager()const { return m_spKeyManager; }
	const std::shared_ptr<CameraManager>& GetCameraManager()const { return m_spCameraManager; }
	const std::unique_ptr<SceneManager>& GetSceneManager()const { return m_upSceneManager; }
	const std::unique_ptr<ImGuiManager>& GetImGuiManager()const { return m_upImGuiManager; }

	void ImGuiReset() { m_upImGuiManager = nullptr; m_upImGuiManager = std::make_unique<ImGuiManager>(); m_upImGuiManager->Init(); }

	Window			m_window;
	FPSController	m_fpsController;

private:
	std::shared_ptr<KeyManager>	m_spKeyManager = nullptr;
	std::shared_ptr<CameraManager>m_spCameraManager = nullptr;
	std::unique_ptr<SceneManager> m_upSceneManager = nullptr;
	std::unique_ptr<ImGuiManager>m_upImGuiManager = nullptr;
	GraphicsDevice* m_pGraphicsDevice;

	Application() {};

public:
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}
};