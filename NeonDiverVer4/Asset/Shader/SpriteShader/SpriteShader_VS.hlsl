#include "inc_SpriteShader.hlsli"
#include "../inc_Common.hlsli"

VSOutput main(float4 pos : POSITION,
              float2 uv : TEXCOORD
)
{
    VSOutput Out;
    Out.Pos = mul(pos, g_mWorld);
    Out.Pos = mul(Out.Pos, g_mProj);
    
    Out.UV = uv;
    
    return Out;
}