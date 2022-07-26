#include "inc_PointShader.hlsli"

// パーティクルの状態についての定数
#define PARTICLE_STATE_DEAD 0
#define PARTICLE_STATE_INIT 1
#define PARTICLE_STATE_UPDATE 2

Texture2D<float4> g_inputTex : register(t1);

SamplerState g_ssClamp : register(s1);

float4 main(PSInput In) : SV_TARGET
{
    float4 color = g_inputTex.Sample(g_ssClamp, In.UV);
    
    if (color.a < 0.1f)
    {
        discard;
    }
    
    color.rgb *= particleData[In.index].col;
    
    return float4(color.rgb, 1.0f);
}