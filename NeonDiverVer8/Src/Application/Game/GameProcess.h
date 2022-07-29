#pragma once

#include "System/Window/Window.h"

class GameManager;
class KeyManager;
class ImGuiManager;

struct FPSController
{
	// FPSêßå‰
	int		m_nowfps = 0;		// åªç›ÇÃFPSíl
	int		m_maxFps = 60;		// ç≈ëÂFPS

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

class GameProcess
{
public:
	GameProcess();
	~GameProcess();

	void StartUp();
	void Run();
	void ShutDown();

	void End() { m_endFlag = true; }

	const std::unique_ptr<GameManager>& GetGameManager()const { return m_upGameManager; }
	const std::shared_ptr<KeyManager>& GetKeyManager()const { return m_spKeyManager; }

	Window		m_window;
	FPSController	m_fpsController;

private:
	void Init(int w, int h);
	void Release();
	void Update();
	void Draw();

	void ImGuiUpdate();

	std::unique_ptr<GameManager>m_upGameManager;
	std::shared_ptr<KeyManager>	m_spKeyManager = nullptr;
	std::unique_ptr<ImGuiManager>m_upImGuiManager = nullptr;

	bool		m_endFlag = false;
};