#include "inc_SkinMeshShader.hlsli"
#include "../inc_Common.hlsli"

VSOutput main(float4 pos : POSITION,
              float2 uv : TEXCOORD,
              float3 normal : NORMAL,
              float4 color : COLOR,
              float3 tangent : TANGENT,
              uint4 skinIndex : SKININDEX,
              float4 skinWeight : SKINWEIGHT
)
{
    // �X�L�j���O----------------
	// �s�������
    row_major float4x4 mBones = 0; // �s���0����
	[unroll]
    for (int i = 0; i < 4; i++)
    {
        mBones += g_mBones[skinIndex[i]] * skinWeight[i];
    }
    
	// ���W�Ɩ@���ɓK�p
    pos = mul(pos, mBones);
    normal = mul(normal, (float3x3) mBones);
    
    VSOutput Out;
    Out.Pos = mul(pos, g_mWorld);
    Out.wPos = Out.Pos.xyz;
    Out.Pos = mul(Out.Pos, g_mView);
    Out.Pos = mul(Out.Pos, g_mProj);
    Out.UV = uv;
    
    Out.Normal = mul(float4(normal, 0), g_mWorld).xyz;
    
    Out.Color = color;
    Out.Tangent = tangent;
    
    // �]�ڐ�
    float3 binormal = cross(normal, tangent);
    Out.biNormal = normalize(mul(binormal, (float3x3) g_mWorld));
    
    return Out;
}