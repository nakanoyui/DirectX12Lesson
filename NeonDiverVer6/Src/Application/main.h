#pragma once

#include "System/Window/Window.h"

class GameSystem;
class KeyManager;

struct FPSController
{
	// FPS制御
	int		m_nowfps = 0;		// 現在のFPS値
	int		m_maxFps = 60;		// 最大FPS

	void Init();

	void UpdateStartTime();

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

	// アプリケーション実行
	void Excute();

	// アプリケーション終了
	void End() { m_endFlag = true; }

	//=====================================================
	// グローバルデータ
	//=====================================================

	// ゲームウィンドウクラス
	Window		m_window;
	FPSController	m_fpsController;

	const std::unique_ptr<GameSystem>&GetGameSystem()const { return m_upGameSystem; }
	const std::shared_ptr<KeyManager>&GetKeyManager()const { return m_spKeyManager; }

private:
	// アプリケーションの初期化
	bool Init(int w, int h);

	// アプリケーション解放
	void Release();

	bool		m_endFlag = false;		// ゲーム終了フラグ trueで終了

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pImguiHeap = nullptr;
		
	std::unique_ptr<GameSystem>	m_upGameSystem = nullptr;
	std::shared_ptr<KeyManager>	m_spKeyManager = nullptr;

	// 
	Application() {};

public:

	//=====================================================
	// シングルトンパターン
	//=====================================================
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}
};