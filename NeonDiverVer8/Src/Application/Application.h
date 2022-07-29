#pragma once

class GameProcess;

class Application
{
public:
	~Application() = default;

	void Excute();

	const std::unique_ptr<GameProcess>& GetGameProcess()const { return m_upGameProcess; }

private:
	std::unique_ptr<GameProcess>m_upGameProcess = nullptr;

	// 
	Application() {};

public:

	//=====================================================
	// シングルトンパターン
	//=====================================================
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}
};