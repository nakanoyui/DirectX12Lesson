#include "DefaultGPUParticleEmitter.h"

DefaultGPUParticleEmitter::DefaultGPUParticleEmitter()
{
	m_spGPUParticleEmitter = std::make_shared<GPUParticleEmitter>();
}

void DefaultGPUParticleEmitter::Set(const EmitterData& emitterData)
{
	m_spGPUParticleEmitter->SetEmitterData(emitterData);
}

void DefaultGPUParticleEmitter::Update(int emitteParticlePerSecond, int particleCyclePerFrame)
{
	m_spGPUParticleEmitter->Update(emitteParticlePerSecond, particleCyclePerFrame);
}

void DefaultGPUParticleEmitter::Draw(const Math::Matrix& mat)
{
	m_spGPUParticleEmitter->Draw(m_wpComputePipeline.lock().get(), mat, m_pTex);
}

void DefaultGPUParticleEmitter::CreateParticleEmitter(GPUParticleManager* pGPUParticleManager, int useParticleValue)
{
	m_spGPUParticleEmitter->Create(pGPUParticleManager,useParticleValue);
}