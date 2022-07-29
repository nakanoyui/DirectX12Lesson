#include "GameProcess.h"

#include "GameManager.h"
#include "System/ImGui/ImGuiManager.h"

GameProcess::GameProcess():
	m_upGameManager(nullptr)
{
}

GameProcess::~GameProcess()
{
}

void GameProcess::StartUp()
{
	Init(static_cast<int>(Constant::ScreenSize.x), static_cast<int>(Constant::ScreenSize.y));
}

void GameProcess::Run()
{
	while (true)
	{
		m_fpsController.UpdateStart();

		m_spKeyManager->Update();

		if (m_spKeyManager->GetKeyState(VK_ESCAPE))
		{
			End();
		}

		// ゲーム終了指定があるときはループ終了
		if (m_endFlag)
		{
			break;
		}

		m_window.ProcessMessage();

		if (!m_window.IsCreated())
		{
			break;
		}

		Update();
		Draw();

		m_upImGuiManager->PreDraw();
		ImGuiUpdate();
		m_upImGuiManager->Draw();

		// フレームレート制御・計測
		m_fpsController.Update();

		GraphicsDevice::Instance().ScreenFlip();
	}
}

void GameProcess::ShutDown()
{
	Release();
}

void GameProcess::Init(int w, int h)
{
	// ウィンドウ作成
	if (!m_window.Create(w, h, "DirectX12", "Window"))
	{
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
	}

	// Direct3D初期化
	GraphicsDevice::Instance().Init(m_window.GetWndHandle(), w, h);

	m_spKeyManager = std::make_shared<KeyManager>();
	m_upGameManager = std::make_unique<GameManager>();
	m_upGameManager->LoadContents();
	m_upGameManager->Init();
	m_upImGuiManager = std::make_unique<ImGuiManager>();
	m_upImGuiManager->Init();

	m_fpsController.Init();
}

void GameProcess::Release()
{
	m_upGameManager = nullptr;
}

void GameProcess::Update()
{
	m_upGameManager->Update();
}

void GameProcess::Draw()
{
	m_upGameManager->Draw();
}

void GameProcess::ImGuiUpdate()
{
	m_upGameManager->ImGuiUpdate();
}

void FPSController::Init()
{
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);//現在の時間を取得（1フレーム目）
}

void FPSController::UpdateStart()
{
	//FPSの計算
	if (iCount == m_maxFps)//カウントが60の時の処理
	{
		QueryPerformanceCounter(&NowTime);//現在の時間を取得（60フレーム目）
		//FPS = 1秒 / 1フレームの描画にかかる時間
		//    = 1000ms / ((現在の時間ms - 1フレーム目の時間ms) / 60フレーム)
		m_nowfps = static_cast <int>(1000.0 / (static_cast<double>((NowTime.QuadPart - StartTime.QuadPart) * 1000.0 / Freq.QuadPart) / m_maxFps));
		iCount = 0;//カウントを初期値に戻す
		StartTime = NowTime;//1フレーム目の時間を現在の時間にする
	}
	iCount++;//カウント+1
}

void FPSController::Update()
{
	//Sleepさせる時間の計算
	QueryPerformanceCounter(&NowTime);//現在の時間を取得
	//Sleepさせる時間ms = 1フレーム目から現在のフレームまでの描画にかかるべき時間ms - 1フレーム目から現在のフレームまで実際にかかった時間ms
	//                  = (1000ms / 60)*フレーム数 - (現在の時間ms - 1フレーム目の時間ms)
	SleepTime = static_cast<DWORD>((1000.0 / m_maxFps) * iCount - (NowTime.QuadPart - StartTime.QuadPart) * 1000 / Freq.QuadPart);
	if (SleepTime > 0 && SleepTime < 18)//大きく変動がなければSleepTimeは1～17の間に納まる
	{
		timeBeginPeriod(1);
		Sleep(SleepTime);
		timeEndPeriod(1);
	}
	else//大きく変動があった場合
	{
		timeBeginPeriod(1);
		Sleep(1);
		timeEndPeriod(1);
	}
}