#include "inc_Particle.hlsli"

// 1スレッドが扱うパーティクルの数
#define PARTICLE_PER_THREAD 256

#define PER_FRAME 1.0f / 60.0f

// パーティクルの状態についての定数
#define PARTICLE_STATE_DEAD 0
#define PARTICLE_STATE_INIT 1
#define PARTICLE_STATE_UPDATE 2

struct EmitterData
{
    float3 startPosition;
    float3 startScale;
    float startRotate;
    
    float MAX_LIFE;
    float3 vel;
    float3 startForce;
    float3 endForce;
    float3 randomMin;
    float3 randomMax;
    
    float3 startColor;
    float3 endColor;
    
    bool sizeByLifeTime;
};

struct ParticleData
{
    int index; // 自身の管理されているエミッター番号  
    int state; // 状態
    float life; // ライフ

	// 基礎データ
    float3 pos;
    float3 scale;
    float rotate;
    float3 vel;
    float3 col;
};

// 全パーティクルデータ
RWStructuredBuffer<ParticleData> particleData : register(u0);

// 全エミッターデータ
StructuredBuffer<EmitterData> emitterData : register(t0);

void InitParticle(uint3 id);
void UpdateParticle(uint3 id);

[numthreads(PARTICLE_PER_THREAD, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    InitParticle(id);
    
    UpdateParticle(id);
}

void InitParticle(uint3 id)
{
    if (particleData[id.x].state != PARTICLE_STATE_INIT)
        return;

    particleData[id.x].pos = emitterData[particleData[id.x].index].startPosition;
    particleData[id.x].scale = emitterData[particleData[id.x].index].startScale;
    particleData[id.x].rotate = emitterData[particleData[id.x].index].startRotate;
    particleData[id.x].col = emitterData[particleData[id.x].index].startColor;
    particleData[id.x].life = 0;
    particleData[id.x].state = PARTICLE_STATE_UPDATE;
    
    // 移動ベクトルだけは疑似ランダムで値を決める
    float3 randomVector;
    randomVector = GetRandomVector(GetRandomVector(id.x), emitterData[particleData[id.x].index].randomMin, emitterData[particleData[id.x].index].randomMax);
    particleData[id.x].vel = randomVector * lerp(emitterData[particleData[id.x].index].startForce, emitterData[particleData[id.x].index].endForce, particleData[id.x].life / emitterData[particleData[id.x].index].MAX_LIFE) * emitterData[particleData[id.x].index].vel;
}

void UpdateParticle(uint3 id)
{
    if (particleData[id.x].state != PARTICLE_STATE_UPDATE)
        return;

    particleData[id.x].pos += particleData[id.x].vel * PER_FRAME;
    particleData[id.x].life++;
    particleData[id.x].col = lerp(emitterData[particleData[id.x].index].startColor, emitterData[particleData[id.x].index].endColor, particleData[id.x].life / emitterData[particleData[id.x].index].MAX_LIFE);

    if (emitterData[particleData[id.x].index].sizeByLifeTime)
        particleData[id.x].scale -= 1 / emitterData[particleData[id.x].index].MAX_LIFE * emitterData[particleData[id.x].index].startScale;
    
    if (particleData[id.x].life > emitterData[particleData[id.x].index].MAX_LIFE)
    {
        particleData[id.x].state = PARTICLE_STATE_DEAD;
        particleData[id.x].pos = float3(100000, 0, 0);
    }
}