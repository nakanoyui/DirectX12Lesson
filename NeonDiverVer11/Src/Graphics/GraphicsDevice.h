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
	// 初期化・解放
	//
	//==============================================================

	// 初期化
	// ・hWnd			… ウィンドウのハンドル
	// ・w				… X解像度
	// ・h				… Y解像度
	bool Init(HWND hWnd, int w, int h);

	void ScreenFlip();

	void WaitForCommandQueue();

	// 解放
	void Release();

	// 白テクスチャ取得
	const Microsoft::WRL::ComPtr<ID3D12Resource>& GetWhiteTex() const { return m_pWhiteTex; }

	// バックバッファのアスペクト比取得
	const float GetAspectRatio()const { return m_aspectRatio; }

	//==============================================================
	// デバッグ
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
	/// レンダ―ターゲットと深度バッファの情報をクリアする命令をコマンドリストにセットする関数
	/// </summary>
	/// <param name="color">指定色で画面を塗りつぶす</param>
	/// <param name="clearFlag">レンダ―ターゲットと深度バッファをクリアするかのフラグ</param>
	/// <param name="pRenderTarget">RenderTargetクラスのポインタ、nullptrの場合このインスタンスが保持しているバックバッファがセットされる</param>
	/// <param name="depthStencil">DepthStencilクラスのポインタ、nullptrの場合このインスタンスが保持している深度バッファがセットされる</param>
	void ClearRenderTarget(const Math::Vector4& color = { 0,0,0,0 }, bool clearFlag = true, RenderTarget* pRenderTarget = nullptr, DepthStencil* pDepthStencil = nullptr);

	/// <summary>
	/// このインスタンスが保持している深度バッファをクリアする
	/// </summary>
	void ClearDepthStencil();

	/// <summary>
	/// リソースとして引数に渡したバッファの扱いをを安全に変更する関数
	/// </summary>
	/// <param name="resource">指定バッファ</param>
	/// <param name="before">現在の状態</param>
	/// <param name="after">新しい状態</param>
	void SetResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	const D3D12_VIEWPORT& GetViewport() { return m_viewport; }

	/// <summary>
	/// ビューポートをセットする関数
	/// </summary>
	/// <param name="size">サイズ</param>
	/// <param name="pos">初期位置</param>
	void SetViewport(const Math::Vector2& size);

	/// <summary>
	/// 白いテクスチャを返す
	/// </summary>
	/// <returns>白いテクスチャ情報</returns>
	ComPtr<ID3D12Resource> CreateWhiteTexture();

	/// <summary>
	/// デバイスのポインタを返す関数
	/// </summary>
	/// <returns>デバイスのポインタ</returns>
	const ComPtr<ID3D12Device>& GetDevice() { return m_pDevice; }

	/// <summary>
	/// コマンドリストのポインタを返す関数
	/// </summary>
	/// <returns>コマンドリストのポインタ</returns>
	const ComPtr<ID3D12GraphicsCommandList>& GetCmdList() { return m_pCmdList; }

	/// <summary>
	/// コマンドキューのポインタを返す関数
	/// </summary>
	/// <returns>コマンドキューのポインタ</returns>
	const ComPtr<ID3D12CommandQueue>& GetCmdQueue() { return m_pCmdQueue; }

	/// <summary>
	/// スワップチェインのポインタを返す関数
	/// </summary>
	/// <returns>スワップチェインを返す関数</returns>
	const ComPtr<IDXGISwapChain4>& GetSwapChain() { return m_pSwapChain; }

	/// <summary>
	/// フレームバッファのポインタを返す関数
	/// </summary>
	/// <returns>フレームバッファのポインタ</returns>
	ID3D12Resource* GetCurrentFrameBuffer() { return m_pFrameBuffers[m_pSwapChain->GetCurrentBackBufferIndex()]; }

	/// <summary>
	/// CBVSRVUAVクラスのポインタを返す関数
	/// </summary>
	/// <returns>CBVSRVUAVクラスのポインタ</returns>
	const std::shared_ptr<CBVSRVUAVHeap>& GetCBVSRVUAVHeap() { return m_spCBVSRVUAVHeap; }

	/// <summary>
	/// 定数バッファアロケーターのポインタを返す関数
	/// </summary>
	/// <returns>定数バッファアロケーターのポインタ</returns>
	const std::shared_ptr<CBufferAllocater>& GetCBufferAllocater() { m_spCBVSRVUAVHeap->Set(); return m_spCBufferAllocater; }

	const std::shared_ptr<GPUParticleManager>&GetGPUParticleManager() { return m_spGPUParticleManager; }

	//-------------------------------
	// シングルトン
	//-------------------------------
private:

	GraphicsDevice() = default;

public:
	static GraphicsDevice& Instance() {
		static GraphicsDevice instance;
		return instance;
	}
};