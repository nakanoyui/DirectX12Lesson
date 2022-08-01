#include "Application.h"

#include "Application/Scene/DebugScene/DebugScene.h"

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
	// �E�B���h�E�쐬
	if (!m_window.Create(static_cast<int>(Constant::ScreenSize.x), static_cast<int>(Constant::ScreenSize.y), "DirectX12", "Window"))
	{
		MessageBoxA(nullptr, "�E�B���h�E�쐬�Ɏ��s", "�G���[", MB_OK);
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

		// �t���[�����[�g����E�v��
		m_fpsController.Update();

		m_pGraphicsDevice->ScreenFlip();

		m_upSceneManager->UpdateLast();
	}
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