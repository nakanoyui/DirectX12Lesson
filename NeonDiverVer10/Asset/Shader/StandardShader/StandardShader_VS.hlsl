#include "inc_StandardShader.hlsli"
#include "../inc_Common.hlsli"

VSOutput main(float4 pos : POSITION,
              float2 uv : TEXCOORD,
              float3 normal : NORMAL,
              float4 color : COLOR,
              float3 tangent : TANGENT
)
{
    VSOutput Out;
    Out.Pos = mul(pos, g_mWorld);
    Out.wPos = Out.Pos.xyz;
    Out.Pos = mul(Out.Pos,g_mView);
    Out.Pos = mul(Out.Pos,g_mProj);
    Out.UV = uv;
    
    Out.Normal = mul(float4(normal, 0), g_mWorld).xyz;
    
    Out.Color = color;
    Out.Tangent = tangent;
    
    // è]ê⁄ê¸
    float3 binormal = cross(normal, tangent);
    Out.biNormal = normalize(mul(binormal, (float3x3) g_mWorld));
    
    return Out;
}