#pragma once

class GameSystem;

struct FPSController
{
	// FPS制御
	int		m_nowfps = 0;		// 現在のFPS値
	int		m_maxFps = 60;		// 最大FPS

	void Init();

	void UpdateStartTime();

	void Update();

private:

	void Control();

	void Monitoring();

	DWORD		m_frameStartTime = 0;		// フレームの開始時間

	int			m_fpsCnt = 0;				// FPS計測用カウント
	DWORD		m_fpsMonitorBeginTime = 0;	// FPS計測開始時間

	const int	kSecond = 1000;				// １秒のミリ秒
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
	const std::shared_ptr<KeyManager>GetKeyManager()const { return m_spKeyManager; }

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