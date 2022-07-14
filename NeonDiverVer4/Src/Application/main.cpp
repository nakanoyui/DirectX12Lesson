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
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);//���݂̎��Ԃ��擾�i1�t���[���ځj
}

void FPSController::UpdateStartTime()
{
	//FPS�̌v�Z
	if (iCount == m_maxFps)//�J�E���g��60�̎��̏���
	{
		QueryPerformanceCounter(&NowTime);//���݂̎��Ԃ��擾�i60�t���[���ځj
		//FPS = 1�b / 1�t���[���̕`��ɂ����鎞��
		//    = 1000ms / ((���݂̎���ms - 1�t���[���ڂ̎���ms) / 60�t���[��)
		m_nowfps = static_cast <int>(1000.0 / (static_cast<double>((NowTime.QuadPart - StartTime.QuadPart) * 1000.0 / Freq.QuadPart) / m_maxFps));
		iCount = 0;//�J�E���g�������l�ɖ߂�
		StartTime = NowTime;//1�t���[���ڂ̎��Ԃ����݂̎��Ԃɂ���
	}
	iCount++;//�J�E���g+1
}

void FPSController::Update()
{
	//Sleep�����鎞�Ԃ̌v�Z
	QueryPerformanceCounter(&NowTime);//���݂̎��Ԃ��擾
	//Sleep�����鎞��ms = 1�t���[���ڂ��猻�݂̃t���[���܂ł̕`��ɂ�����ׂ�����ms - 1�t���[���ڂ��猻�݂̃t���[���܂Ŏ��ۂɂ�����������ms
	//                  = (1000ms / 60)*�t���[���� - (���݂̎���ms - 1�t���[���ڂ̎���ms)
	SleepTime = static_cast<DWORD>((1000.0 / m_maxFps) * iCount - (NowTime.QuadPart - StartTime.QuadPart) * 1000 / Freq.QuadPart);
	if (SleepTime > 0 && SleepTime < 18)//�傫���ϓ����Ȃ����SleepTime��1�`17�̊Ԃɔ[�܂�
	{
		timeBeginPeriod(1);
		Sleep(SleepTime);
		timeEndPeriod(1);
	}
	else//�傫���ϓ����������ꍇ
	{
		timeBeginPeriod(1);
		Sleep(1);
		timeEndPeriod(1);
	}
}