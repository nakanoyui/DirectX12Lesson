#pragma once

class DepthStencil
{
public:
	DepthStencil() = default;
	virtual ~DepthStencil();

	/// <summary>
	/// DSVとして扱うGPUバッファの生成関数
	/// </summary>
	/// <param name="pGraphicsDevice">GraphicsDeviceのポインタ</param>
	/// <param name="size">解像度</param>
	void CreateBuffer(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size);

	/// <summary>
	/// セットされている深度バッファのポインタを返す関数
	/// </summary>
	/// <returns>セットされている深度バッファのポインタ</returns>
	ID3D12Resource* GetResource() { return m_pDepthBuffer; }

	/// <summary>
	/// セットされているヒープのポインタを返す関数
	/// </summary>
	/// <returns>セットされているヒープのポインタ</returns>
	ID3D12DescriptorHeap* GetHeap() { return m_pHeap; }

	/// <summary>
	/// 深度バッファに書き込まれているデータを初期化する関数
	/// </summary>
	void Clear();

protected:
	GraphicsDevice* m_pGraphicsDevice = nullptr;
	ID3D12Resource* m_pDepthBuffer = nullptr;
	ID3D12DescriptorHeap* m_pHeap = nullptr;
};