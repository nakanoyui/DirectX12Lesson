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

		// �Q�[���I���w�肪����Ƃ��̓��[�v�I��
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

		// �t���[�����[�g����E�v��
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
	// �E�B���h�E�쐬
	if (!m_window.Create(w, h, "DirectX12", "Window"))
	{
		MessageBoxA(nullptr, "�E�B���h�E�쐬�Ɏ��s", "�G���[", MB_OK);
	}

	// Direct3D������
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
	QueryPerformanceCounter(&StartTime);//���݂̎��Ԃ��擾�i1�t���[���ځj
}

void FPSController::UpdateStart()
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