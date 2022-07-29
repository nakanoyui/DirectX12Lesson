#pragma once

#include "GPUParticleEmitter.h"

class DefaultGPUParticleEmitter
{
public:
	DefaultGPUParticleEmitter();
	~DefaultGPUParticleEmitter() = default;
	void Set(const EmitterData&emitterData);
	void Update(int emitteParticlePerSecond,int particleCyclePerFrame);
	void Draw(const Math::Matrix& mat);
	void CreateParticleEmitter(GPUParticleManager* pGPUParticleManager, int useParticleValue);
	void SetComputeShader(const std::shared_ptr<ComputePipeline>& spSetShader) { m_wpComputePipeline = spSetShader; }
	void SetTexture(Texture* pTex) { m_pTex = pTex; }

protected:
	EmitterData* m_pEmitterData = nullptr;
	Texture* m_pTex = nullptr;
private:
	std::shared_ptr<GPUParticleEmitter> m_spGPUParticleEmitter = nullptr;
	std::weak_ptr<ComputePipeline> m_wpComputePipeline;
};