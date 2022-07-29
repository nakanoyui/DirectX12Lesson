#pragma once

#include "GPUParticleManager.h"

class GPUParticleEmitter
{
public:
	GPUParticleEmitter();
	~GPUParticleEmitter();

	void Create(GPUParticleManager* pManager, UINT useParticleValue);
	void Update(UINT addParticleValue, UINT addCyclePerFrame);
	void Draw(ComputePipeline* pComputePipeline, const Math::Matrix& mat, Texture* pTex = nullptr);
	
	void SetEmitterData(const EmitterData& emitterData) { m_pEmitterData[m_registValue] = emitterData; }

private:
	enum class ParticleState
	{
		Dead,
		Init,
		Update,
	};

	GPUParticleManager* m_pManager = nullptr;
	ParticleData* m_upParticleData = nullptr;
	ParticleData* m_upUpdateParticleData = nullptr;
	UINT m_useParticleOffset;
	UINT m_useParticleValue;
	UINT m_uavValue;
	UINT m_srvValue;
	UINT m_registValue;		// 自分のエミッター番号
	EmitterData* m_pEmitterData = nullptr;
	UINT m_count;

	std::weak_ptr<AssetManager>m_wpAssetManager;
};