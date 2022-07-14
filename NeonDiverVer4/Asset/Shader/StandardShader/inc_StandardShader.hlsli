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
    float4 g_BaseColor; // �x�[�X�F
    float3 g_Emissive;  // ���Ȕ����F
    float g_Metallic;   // �����x
    float g_Roughness;  // �e��
}