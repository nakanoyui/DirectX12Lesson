#include "Application.h"

#include "Game/GameProcess.h"

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
	m_upGameProcess = std::make_unique<GameProcess>();
	m_upGameProcess->StartUp();
	m_upGameProcess->Run();
	m_upGameProcess->ShutDown();
}