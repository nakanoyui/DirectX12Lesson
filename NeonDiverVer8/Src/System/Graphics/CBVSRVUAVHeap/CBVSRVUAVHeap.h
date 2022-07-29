#pragma once

class CBVSRVUAVHeap
{
public:
	CBVSRVUAVHeap() = default;
	~CBVSRVUAVHeap();

	/// <summary>
	/// デバイスのポインタをセットする関数
	/// </summary>
	/// <param name="pDevice">デバイスのポインタ</param>
	/// <param name="pCmdList">コマンドリストのポインタ</param>
	void SetGraphicsDevice(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCmdList);

	/// <summary>
	/// ヒープの生成関数
	/// </summary>
	/// <param name="useCount">CBV、SRV、UAVの順番でいくつのデータを格納できるようにするか設定する値</param>
	void Create(const Math::Vector3& useCount);

	/// <summary>
	/// ディスクリプタヒープのポインタを返す関数
	/// </summary>
	/// <returns>ディスクリプタヒープのポインタ</returns>
	ID3D12DescriptorHeap* GetHeap() { return m_pHeap; }

	/// <summary>
	/// CBV、SRV、UAVの順番でいくつのデータを格納できるか返す関数
	/// </summary>
	/// <returns>各データの許容値</returns>
	Math::Vector3 GetUseCount() { return m_useCount; }

	/// <summary>
	/// 引数で渡されたGPUバッファからShaderResourceViewを生成する関数
	/// </summary>
	/// <param name="pBuffer">GPUBuffer</param>
	void CreateSRV(ID3D12Resource* pBuffer);

	/// <summary>
	/// 引数で渡されたGPUバッファと指定されたViewの生成データからShaderResourceViewを生成する関数
	/// </summary>
	/// <param name="pBuffer">GPUBuffer</param>
	/// <param name="viewDesc">シェーダーリソースビュー情報</param>
	void CreateSRV(ID3D12Resource* pBuffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc);

	/// <summary>
	/// SRVのGPU側アドレスを返す関数
	/// </summary>
	/// <param name="number">テクスチャ番号</param>
	/// <returns>SRVのGPU側アドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleForSRV(int number);

	/// <summary>
	/// 次に生成されるべきSRV用のディスクリプタヒープの番地を返す関数
	/// </summary>
	/// <returns>次に生成されるべきSRV用のディスクリプタヒープの番地</returns>
	int GetNextSrvNumber() { return m_nextSrvDescriptorNum; }

	/// <summary>
	/// ヒープをセットする関数
	/// </summary>
	void Set();
private:
	ID3D12Device* m_pDevice = nullptr;
	ID3D12GraphicsCommandList* m_pCmdList = nullptr;
	ID3D12DescriptorHeap* m_pHeap = nullptr;
	Math::Vector3 m_useCount;
	int m_incrementSize = 0;
	int m_nextSrvDescriptorNum = 0;
};