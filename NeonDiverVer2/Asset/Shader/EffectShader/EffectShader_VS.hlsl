#include "inc_EffectShader.hlsli"
#include "../inc_Common.hlsli"

VSOutput main(float4 pos : POSITION, float4 color : COLOR)
{
    VSOutput Out;
    Out.Pos = mul(pos,g_mWorld);
    Out.Pos = mul(Out.Pos, g_mView);
    Out.Pos = mul(Out.Pos, g_mProj);
	
    Out.Color = color;
    
    return Out;
}