#include "main.h"

#include "GameSystem/GameSystem.h"

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
	//===================================================================
	// 初期設定(ウィンドウ作成、Direct3D初期化など)
	//===================================================================
	if (!Application::Instance().Init(static_cast<int>(Constant::ScreenSize.x), static_cast<int>(Constant::ScreenSize.y)))
	{
		return;
	}

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	GraphicsDevice::Instance().GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pImguiHeap));

	bool imguiCheck = true;
	IMGUI_CHECKVERSION();
	if (!ImGui::CreateContext())imguiCheck = false;
	imguiCheck = ImGui_ImplWin32_Init(m_window.GetWndHandle());
	imguiCheck = ImGui_ImplDX12_Init(GraphicsDevice::Instance().GetDevice().Get(), 2
		, GraphicsDevice::Instance().GetCurrentFrameBuffer()->GetDesc().Format, m_pImguiHeap.Get()
		, m_pImguiHeap->GetCPUDescriptorHandleForHeapStart(), m_pImguiHeap->GetGPUDescriptorHandleForHeapStart());

	m_fpsController.Init();

	m_spKeyManager = std::make_shared<KeyManager>();

	m_upGameSystem = std::make_unique<GameSystem>();
	m_upGameSystem->LoadContents();
	m_upGameSystem->Init();

	// ループ
	while (true)
	{
		// 処理開始時間Get
		m_fpsController.UpdateStartTime();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			m_endFlag = true;
		}

		// ゲーム終了指定があるときはループ終了
		if (m_endFlag)
		{
			break;
		}

		//=========================================
		//
		// ウィンドウ関係の処理
		//
		//=========================================

		// ウィンドウのメッセージを処理する
		m_window.ProcessMessage();

		// ウィンドウが破棄されてるならループ終了
		if (m_window.IsCreated() == false)
		{
			break;
		}

		//=========================================
		//
		// ゲーム処理
		//
		//=========================================

		m_spKeyManager->Update();

		m_upGameSystem->Update();

		m_upGameSystem->Draw();

		GraphicsDevice::Instance().GetCBufferAllocater()->ResetCurrentUseNumber();
		GraphicsDevice::Instance().GetCBVSRVUAVHeap()->Set();

		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(20, 20));
		ImGui::SetNextWindowSize(ImVec2(200, 150));

		ImGui::Begin("System");
		ImGui::Text("FPS : %d", m_fpsController.m_nowfps);
		m_upGameSystem->ImGuiUpdate();
		ImGui::End();

		ImGui::Render();
		ID3D12DescriptorHeap* ppHeaps[] = { m_pImguiHeap.Get() };
		GraphicsDevice::Instance().GetCmdList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GraphicsDevice::Instance().GetCmdList().Get());

		GraphicsDevice::Instance().ScreenFlip();

		//=========================================
		//
		// フレームレート制御・計測
		//
		//=========================================
		m_fpsController.Update();
	}

	//===================================================================
	// アプリケーション解放
	//===================================================================
	Release();
}

bool Application::Init(int w, int h)
{
	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (!m_window.Create(w, h, "DirectX12", "Window"))
	{
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}

	// Direct3D初期化
	GraphicsDevice::Instance().Init(m_window.GetWndHandle(), w, h);

	return true;
}

void Application::Release()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void FPSController::Init()
{
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);//現在の時間を取得（1フレーム目）
}

void FPSController::UpdateStartTime()
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