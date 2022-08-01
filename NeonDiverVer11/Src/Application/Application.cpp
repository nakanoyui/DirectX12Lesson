#include "Application.h"

#include "Application/Scene/DebugScene/DebugScene.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// COM初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL, "japanese");

	Application::Instance().Excute();

	// COM解放
	CoUninitialize();

	return 0;
}

void Application::Excute()
{
	// ウィンドウ作成
	if (!m_window.Create(static_cast<int>(Constant::ScreenSize.x), static_cast<int>(Constant::ScreenSize.y), "DirectX12", "Window"))
	{
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
	}

	GraphicsDevice::Instance().Init(m_window.GetWndHandle(), static_cast<int>(Constant::ScreenSize.x), static_cast<int>(Constant::ScreenSize.y));

	m_pGraphicsDevice = &GraphicsDevice::Instance();

	m_spKeyManager = std::make_shared<KeyManager>();

	m_spCameraManager = std::make_shared<CameraManager>();

	m_upImGuiManager = std::make_unique<ImGuiManager>();
	m_upImGuiManager->Init();

	m_upSceneManager = std::make_unique<SceneManager>();
	m_upSceneManager->Init(std::make_shared<DebugScene>());

	m_fpsController.Init();

	while (true)
	{
		m_fpsController.UpdateStart();

		m_spKeyManager->Update();

		if (m_spKeyManager->GetKeyState(VK_ESCAPE))
		{
			break;
		}

		m_window.ProcessMessage();

		if (!m_window.IsCreated())
		{
			break;
		}

		m_upSceneManager->Update();
		m_spCameraManager->Update();
		m_upSceneManager->Draw();

		m_upImGuiManager->PreDraw();
		m_upSceneManager->ImGuiUpdate();
		m_upImGuiManager->Draw();

		// フレームレート制御・計測
		m_fpsController.Update();

		m_pGraphicsDevice->ScreenFlip();

		m_upSceneManager->UpdateLast();
	}
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