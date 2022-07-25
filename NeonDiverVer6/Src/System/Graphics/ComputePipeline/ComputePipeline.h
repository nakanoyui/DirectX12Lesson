#pragma once

class ComputePipeline
{
public:
	/// <summary>
	/// コンストラクタ兼シェーダーコンパイル
	/// </summary>
	/// <param name="pGraphicsDevice">GraphicsDeviceのポインタ</param>
	/// <param name="filepath">コンパイルするシェーダーファイルの名前</param>
	ComputePipeline(GraphicsDevice* pGraphicsDevice, const std::wstring& filepath);
	~ComputePipeline();

	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="range">シェーダーで扱うRootParamの配列</param>
	void Create(const std::vector<RangeType>& range);

	/// <summary>
	/// シェーダーをセットする関数
	/// </summary>
	void Set();
private:
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	GraphicsDevice* m_pGraphicsDevice = nullptr;
	ComPtr<ID3DBlob>m_pCSBlob = nullptr;
	ComPtr<ID3D12PipelineState>m_pPipeline = nullptr;
	std::unique_ptr<RootSignature> m_upRootSignature = nullptr;
};