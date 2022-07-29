#include "inc_EffectShader.hlsli"
#include "../inc_Common.hlsli"

float4 main(VSOutput In) : SV_TARGET
{
    return In.Color;
}