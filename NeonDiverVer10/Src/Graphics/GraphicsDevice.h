#pragma once

#include "CBVSRVUAVHeap/CBVSRVUAVHeap.h"

class RenderTarget;
class RenderTexture;
class DepthStencil;
class Shader;
class Mesh;
class CBufferAllocater;
class GPUParticleManager;

class GraphicsDevice
{
public:
	~GraphicsDevice() { Release(); }

	//==============================================================
	//
	// �������E���
	//
	//==============================================================

	// ������
	// �EhWnd			�c �E�B���h�E�̃n���h��
	// �Ew				�c X�𑜓x
	// �Eh				�c Y�𑜓x
	bool Init(HWND hWnd, int w, int h);

	void ScreenFlip();

	void WaitForCommandQueue();

	// ���
	void Release();

	// ���e�N�X�`���擾
	const Microsoft::WRL::ComPtr<ID3D12Resource>& GetWhiteTex() const { return m_pWhiteTex; }

	// �o�b�N�o�b�t�@�̃A�X�y�N�g��擾
	const float GetAspectRatio()const { return m_aspectRatio; }

	//==============================================================
	// �f�o�b�O
	//==============================================================
	void EnableDebugLayer();

private:
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_VIEWPORT						m_viewport = {};
	D3D12_RECT							m_rect = {};
	float								m_aspectRatio;
	ComPtr<ID3D12Device>				m_pDevice = nullptr;
	ComPtr<IDXGIFactory6>				m_pDxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4>				m_pSwapChain = nullptr;

	ComPtr<ID3D12CommandAllocator>		m_pCmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList>	m_pCmdList = nullptr;
	ComPtr<ID3D12CommandQueue>			m_pCmdQueue = nullptr;

	std::vector<ID3D12Resource*>		m_pFrameBuffers;
	std::unique_ptr<RenderTarget>		m_upSwapChainRT = nullptr;
	ComPtr<ID3D12DescriptorHeap>		m_pDsvHeap = nullptr;
	ComPtr<ID3D12Resource>				m_pDepthBuffer = nullptr;
	ComPtr<ID3D12Fence>					m_pFence = nullptr;
	UINT64								m_fenceVal = 0;

	std::shared_ptr<CBVSRVUAVHeap>		m_spCBVSRVUAVHeap = nullptr;
	std::shared_ptr<CBufferAllocater>	m_spCBufferAllocater = nullptr;

	ComPtr<ID3D12Resource>				m_pWhiteTex = nullptr;

	std::shared_ptr<GPUParticleManager> m_spGPUParticleManager = nullptr;

private:
	bool CreateFactory();
	bool CreateDevice();
	bool CreateCommandList();
	bool CreateSwapChain(HWND hwnd, const int& width, const int& height);
	bool CreateSwapChainRTV();
	bool CreateDsv();
	bool CreateFence();
public:
	/// <summary>
	/// �����_�\�^�[�Q�b�g�Ɛ[�x�o�b�t�@�̏����N���A���閽�߂��R�}���h���X�g�ɃZ�b�g����֐�
	/// </summary>
	/// <param name="color">�w��F�ŉ�ʂ�h��Ԃ�</param>
	/// <param name="clearFlag">�����_�\�^�[�Q�b�g�Ɛ[�x�o�b�t�@���N���A���邩�̃t���O</param>
	/// <param name="pRenderTarget">RenderTarget�N���X�̃|�C���^�Anullptr�̏ꍇ���̃C���X�^���X���ێ����Ă���o�b�N�o�b�t�@���Z�b�g�����</param>
	/// <param name="depthStencil">DepthStencil�N���X�̃|�C���^�Anullptr�̏ꍇ���̃C���X�^���X���ێ����Ă���[�x�o�b�t�@���Z�b�g�����</param>
	void ClearRenderTarget(const Math::Vector4& color = { 0,0,0,0 }, bool clearFlag = true, RenderTarget* pRenderTarget = nullptr, DepthStencil* pDepthStencil = nullptr);

	/// <summary>
	/// ���̃C���X�^���X���ێ����Ă���[�x�o�b�t�@���N���A����
	/// </summary>
	void ClearDepthStencil();

	/// <summary>
	/// ���\�[�X�Ƃ��Ĉ����ɓn�����o�b�t�@�̈����������S�ɕύX����֐�
	/// </summary>
	/// <param name="resource">�w��o�b�t�@</param>
	/// <param name="before">���݂̏��</param>
	/// <param name="after">�V�������</param>
	void SetResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	const D3D12_VIEWPORT& GetViewport() { return m_viewport; }

	/// <summary>
	/// �r���[�|�[�g���Z�b�g����֐�
	/// </summary>
	/// <param name="size">�T�C�Y</param>
	/// <param name="pos">�����ʒu</param>
	void SetViewport(const Math::Vector2& size);

	/// <summary>
	/// �����e�N�X�`����Ԃ�
	/// </summary>
	/// <returns>�����e�N�X�`�����</returns>
	ComPtr<ID3D12Resource> CreateWhiteTexture();

	/// <summary>
	/// �f�o�C�X�̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>�f�o�C�X�̃|�C���^</returns>
	const ComPtr<ID3D12Device>& GetDevice() { return m_pDevice; }

	/// <summary>
	/// �R�}���h���X�g�̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>�R�}���h���X�g�̃|�C���^</returns>
	const ComPtr<ID3D12GraphicsCommandList>& GetCmdList() { return m_pCmdList; }

	/// <summary>
	/// �R�}���h�L���[�̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>�R�}���h�L���[�̃|�C���^</returns>
	const ComPtr<ID3D12CommandQueue>& GetCmdQueue() { return m_pCmdQueue; }

	/// <summary>
	/// �X���b�v�`�F�C���̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>�X���b�v�`�F�C����Ԃ��֐�</returns>
	const ComPtr<IDXGISwapChain4>& GetSwapChain() { return m_pSwapChain; }

	/// <summary>
	/// �t���[���o�b�t�@�̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>�t���[���o�b�t�@�̃|�C���^</returns>
	ID3D12Resource* GetCurrentFrameBuffer() { return m_pFrameBuffers[m_pSwapChain->GetCurrentBackBufferIndex()]; }

	/// <summary>
	/// CBVSRVUAV�N���X�̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>CBVSRVUAV�N���X�̃|�C���^</returns>
	const std::shared_ptr<CBVSRVUAVHeap>& GetCBVSRVUAVHeap() { return m_spCBVSRVUAVHeap; }

	/// <summary>
	/// �萔�o�b�t�@�A���P�[�^�[�̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>�萔�o�b�t�@�A���P�[�^�[�̃|�C���^</returns>
	const std::shared_ptr<CBufferAllocater>& GetCBufferAllocater() { m_spCBVSRVUAVHeap->Set(); return m_spCBufferAllocater; }

	const std::shared_ptr<GPUParticleManager>&GetGPUParticleManager() { return m_spGPUParticleManager; }

	//-------------------------------
	// �V���O���g��
	//-------------------------------
private:

	GraphicsDevice() = default;

public:
	static GraphicsDevice& Instance() {
		static GraphicsDevice instance;
		return instance;
	}
};