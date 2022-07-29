#include "Application.h"

#include "Game/GameProcess.h"

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
	m_upGameProcess = std::make_unique<GameProcess>();
	m_upGameProcess->StartUp();
	m_upGameProcess->Run();
	m_upGameProcess->ShutDown();
}