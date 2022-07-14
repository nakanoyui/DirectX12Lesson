#include "inc_SpriteShader.hlsli"

Texture2D<float4> g_inputTex : register(t0); // 入力テクスチャ
SamplerState g_ss : register(s0); // サンプラ

float4 main(VSOutput In) : SV_Target0
{
    float4 color = g_inputTex.Sample(g_ss, In.UV);
    
    if (color.a < 0.1f)
        discard;
    
    return color;
}