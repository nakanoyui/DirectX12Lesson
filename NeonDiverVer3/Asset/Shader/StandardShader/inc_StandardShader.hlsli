struct VSOutput
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD0;
    float3 wPos : TEXCOORD1;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float4 Color : COLOR;
    float3 biNormal : BINORMAL;
};

cbuffer cbWorld : register(b2)
{
    row_major matrix g_mWorld;
}

cbuffer cbMaterial : register(b3)
{
    float4 g_BaseColor; // ベース色
    float3 g_Emissive;  // 自己発光色
    float g_Metallic;   // 金属度
    float g_Roughness;  // 粗さ
}