#pragma once

class GameSystem;

struct FPSController
{
	// FPS����
	int		m_nowfps = 0;		// ���݂�FPS�l
	int		m_maxFps = 60;		// �ő�FPS

	void Init();

	void UpdateStartTime();

	void Update();

private:

	void Control();

	void Monitoring();

	DWORD		m_frameStartTime = 0;		// �t���[���̊J�n����

	int			m_fpsCnt = 0;				// FPS�v���p�J�E���g
	DWORD		m_fpsMonitorBeginTime = 0;	// FPS�v���J�n����

	const int	kSecond = 1000;				// �P�b�̃~���b
};

class Application
{
public:
	~Application() = default;

	// �A�v���P�[�V�������s
	void Excute();

	// �A�v���P�[�V�����I��
	void End() { m_endFlag = true; }

	//=====================================================
	// �O���[�o���f�[�^
	//=====================================================

	// �Q�[���E�B���h�E�N���X
	Window		m_window;
	FPSController	m_fpsController;

	const std::unique_ptr<GameSystem>&GetGameSystem()const { return m_upGameSystem; }
	const std::shared_ptr<KeyManager>GetKeyManager()const { return m_spKeyManager; }

private:
	// �A�v���P�[�V�����̏�����
	bool Init(int w, int h);

	// �A�v���P�[�V�������
	void Release();

	bool		m_endFlag = false;		// �Q�[���I���t���O true�ŏI��

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pImguiHeap = nullptr;
		
	std::unique_ptr<GameSystem>	m_upGameSystem = nullptr;

	std::shared_ptr<KeyManager>	m_spKeyManager = nullptr;

	// 
	Application() {};

public:

	//=====================================================
	// �V���O���g���p�^�[��
	//=====================================================
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}
};