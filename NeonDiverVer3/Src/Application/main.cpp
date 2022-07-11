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
	m_fpsMonitorBeginTime = timeGetTime();
}

void FPSController::UpdateStartTime()
{
	m_frameStartTime = timeGetTime();
}

void FPSController::Update()
{
	Control();

	Monitoring();
}

void FPSController::Control()
{
	// 処理終了時間Get
	DWORD frameProcessEndTime = timeGetTime();

	// 1フレームで経過すべき時間
	DWORD timePerFrame = kSecond / m_maxFps;

	if (frameProcessEndTime - m_frameStartTime < timePerFrame)
	{
		// 1秒間にMaxFPS回数以上処理が回らないように待機する
		Sleep(timePerFrame - (frameProcessEndTime - m_frameStartTime));
	}
}

void FPSController::Monitoring()
{
	constexpr float kFpsRefreshFrame = 500;		// FPS計測のタイミング　0.5秒おき

	m_fpsCnt++;

	// 0.5秒おきに FPS計測
	if (m_frameStartTime - m_fpsMonitorBeginTime >= kFpsRefreshFrame)
	{
		// 現在のFPS算出
		m_nowfps = (m_fpsCnt * kSecond) / (m_frameStartTime - m_fpsMonitorBeginTime);

		m_fpsMonitorBeginTime = m_frameStartTime;

		m_fpsCnt = 0;
	}
}