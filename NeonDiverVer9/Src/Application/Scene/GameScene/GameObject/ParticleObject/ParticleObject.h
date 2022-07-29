#pragma once

#include "System/GPUParticle/DefaultGPUParticleEmitter.h"

class ParticleObject :public GameObject
{
public:
	ParticleObject() = default;
	virtual ~ParticleObject() = default;

	virtual void LoadAsset() {}

	virtual nlohmann::json SetJsonParameter();
	virtual void Init();

	void SliderInt(const char* label,int& in,int min,int max);
	void SliderFloat(const char* label,float& in,float min,float max);
	void SliderFloat3(const char* label,Math::Vector3& in,float min,float max);
	void ColorEdit3(const char* label,Math::Vector3& in);

protected:
	void GeneralInit();
	void EmitteInit();

	std::unique_ptr<DefaultGPUParticleEmitter> m_upParticleEmitter = nullptr;
	std::weak_ptr<AssetManager>m_wpAssetManager;
	std::weak_ptr<JsonManager> m_wpJsonManager;
	std::weak_ptr<Json>m_wpJson;
	
	Math::Vector3 m_euler;

	int m_emitteParticlePerSecond;		// 1�t���[���ŕ��o����p�[�e�B�N����
	int m_particleCyclePerFrame;		// ���t���[���Ńp�[�e�B�N������o�����邩(0��1�͖��t���[��)
	EmitterData m_emitterData;

	float m_particleNum;

	bool m_bDefaultTexture;
	bool m_bLaserTexture;
};