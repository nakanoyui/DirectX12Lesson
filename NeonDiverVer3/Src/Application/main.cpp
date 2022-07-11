#include "main.h"

#include "GameSystem/GameSystem.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ���������[�N��m�点��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// COM������
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	// mbstowcs_s�֐��œ��{��Ή��ɂ��邽�߂ɌĂ�
	setlocale(LC_ALL, "japanese");

	Application::Instance().Excute();

	// COM���
	CoUninitialize();

	return 0;
}

void Application::Excute()
{
	//===================================================================
	// �����ݒ�(�E�B���h�E�쐬�ADirect3D�������Ȃ�)
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

	// ���[�v
	while (true)
	{
		// �����J�n����Get
		m_fpsController.UpdateStartTime();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			m_endFlag = true;
		}

		// �Q�[���I���w�肪����Ƃ��̓��[�v�I��
		if (m_endFlag)
		{
			break;
		}

		//=========================================
		//
		// �E�B���h�E�֌W�̏���
		//
		//=========================================

		// �E�B���h�E�̃��b�Z�[�W����������
		m_window.ProcessMessage();

		// �E�B���h�E���j������Ă�Ȃ烋�[�v�I��
		if (m_window.IsCreated() == false)
		{
			break;
		}

		//=========================================
		//
		// �Q�[������
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
		// �t���[�����[�g����E�v��
		//
		//=========================================
		m_fpsController.Update();
	}

	//===================================================================
	// �A�v���P�[�V�������
	//===================================================================
	Release();
}

bool Application::Init(int w, int h)
{
	//===================================================================
	// �E�B���h�E�쐬
	//===================================================================
	if (!m_window.Create(w, h, "DirectX12", "Window"))
	{
		MessageBoxA(nullptr, "�E�B���h�E�쐬�Ɏ��s", "�G���[", MB_OK);
		return false;
	}

	// Direct3D������
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
	// �����I������Get
	DWORD frameProcessEndTime = timeGetTime();

	// 1�t���[���Ōo�߂��ׂ�����
	DWORD timePerFrame = kSecond / m_maxFps;

	if (frameProcessEndTime - m_frameStartTime < timePerFrame)
	{
		// 1�b�Ԃ�MaxFPS�񐔈ȏ㏈�������Ȃ��悤�ɑҋ@����
		Sleep(timePerFrame - (frameProcessEndTime - m_frameStartTime));
	}
}

void FPSController::Monitoring()
{
	constexpr float kFpsRefreshFrame = 500;		// FPS�v���̃^�C�~���O�@0.5�b����

	m_fpsCnt++;

	// 0.5�b������ FPS�v��
	if (m_frameStartTime - m_fpsMonitorBeginTime >= kFpsRefreshFrame)
	{
		// ���݂�FPS�Z�o
		m_nowfps = (m_fpsCnt * kSecond) / (m_frameStartTime - m_fpsMonitorBeginTime);

		m_fpsMonitorBeginTime = m_frameStartTime;

		m_fpsCnt = 0;
	}
}