#pragma once

struct EmitterData
{
	Math::Vector3 m_startPosition;
	Math::Vector3 m_startScale = Math::Vector3(Constant::One_F);
	float m_startRotate = Constant::Zero_F;

	float MAX_LIFE = 180;
	Math::Vector3 m_velocity = Math::Vector3(Constant::One_F);
	Math::Vector3 m_startForce = Math::Vector3(Constant::One_F);
	Math::Vector3 m_endForce = Math::Vector3(Constant::One_F);
	Math::Vector3 m_randomMin = Math::Vector3(-Constant::One_F, Constant::One_F, -Constant::One_F);
	Math::Vector3 m_randomMax = Math::Vector3(Constant::One_F);

	Math::Vector3 m_startColor = Math::Vector3(Constant::One_F);
	Math::Vector3 m_endColor = Math::Vector3(Constant::One_F);

	bool m_sizeByLifeTime = false;
};

struct ParticleData
{
	int m_index;			// 自身のエミッター管理番号
	int m_state = Constant::Zero_I;		// 状態
	float m_life;			// ライフ

	// 基礎データ
	Math::Vector3 m_pos;
	Math::Vector3 m_scale;
	float m_rotate;
	Math::Vector3 m_velocity;
	Math::Vector3 m_color;
};

class GPUParticleManager
{
private:
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	GPUParticleManager(GraphicsDevice* pGraphicsDevice);
	~GPUParticleManager();
	UINT GetMaxParticleValue() { return MaxParticleValue; }
	UINT GetNextParticleOffset() { return m_nextParticleOffset; }
	UINT GetUAVNextOffset() { return m_uavNextOffset; }
	UINT GetSRVNextOffset() { return m_srvNextOffset; }
	UINT GetEmitterNextOffset() { return m_emitterNextOffset++; }
	UINT GetEmitterDataSRVValue() { return m_emitterDataSRVValue; }
	ComPtr<ID3D12Resource> GetParticleBuffer() { return m_pParticleBuffer; }
	ComPtr<ID3D12Resource> GetUpdateParticleBuffer() { return m_pUpdateParticleBuffer; }
	ComPtr<ID3D12Resource> GetEmitterParticleBuffer() { return m_pEmitterParticleBuffer; }
	ComPtr<ID3D12DescriptorHeap> GetHeap() { return m_pHeap; }

	void IncrementParticleOffset(UINT value) { m_nextParticleOffset += value; }
	void CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& viewDesc);
	void CreateSRV(ID3D12Resource* pBuffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc);

	GraphicsDevice* GetDevice() { return m_pGraphicsDevice; }

	void Update();
	void Dispatch(ComputePipeline* pShader);

private:
	const UINT MaxParticleValue;		// アプリケーションが扱えるパーティクル上限
	UINT m_nextParticleOffset;			// 次のエミッターが扱えるパーティクルバッファーの先頭アドレスを指す変数
	const UINT MaxEmitterValue;			// エミッターの最大数
	UINT m_uavNextOffset;				// コンピュートシェーダーで扱うUAVの次のアドレスを指すための変数
	UINT m_srvNextOffset;				// 描画で扱うSRVの次のアドレスを指すための変数

	UINT m_emitterDataSRVValue;			// エミッターSRVの番号
	UINT m_emitterNextOffset;			// 自分のエミッター番号を指す変数

	ComPtr<ID3D12Resource> m_pParticleBuffer = nullptr;
	ComPtr<ID3D12Resource> m_pUpdateParticleBuffer = nullptr;
	ComPtr<ID3D12Resource> m_pEmitterParticleBuffer = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_pHeap = nullptr;

	GraphicsDevice* m_pGraphicsDevice = nullptr;
};