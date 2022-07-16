#include "inc_StandardShader.hlsli"
#include "../inc_Common.hlsli"

Texture2D g_albedoTex : register(t0); // �A���x�h�e�N�X�`��
Texture2D g_normalMap : register(t1); // �@���}�b�v
Texture2D g_metalRoughMap : register(t2); // ���^���b�N/���t�l�X�e�N�X�`��
Texture2D g_emissiveMap : register(t3); // �����e�N�X�`��

SamplerState g_ss : register(s0); // �T���v��

static const float PI = 3.1415926f; // ��

float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normalMap.Sample(g_ss, uv).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

    return newNormal;
}

// �x�b�N�}�����z���v�Z����
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

// �t���l�����v�Z�BSchlick�ߎ����g�p
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// Cook-Torrance���f���̋��ʔ��˂��v�Z
/// </summary>
/// <param name="L">�����Ɍ������x�N�g��</param>
/// <param name="V">���_�Ɍ������x�N�g��</param>
/// <param name="N">�@���x�N�g��</param>
/// <param name="metallic">�����x</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic)
{
    float microfacet = 0.76f;

    // �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
    // �����x�������قǃt���l�����˂͑傫���Ȃ�
    float f0 = metallic;

    // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D�����x�b�N�}�����z��p���Čv�Z����
    float D = Beckmann(microfacet, NdotH);

    // F����Schlick�ߎ���p���Čv�Z����
    float F = SpcFresnel(f0, VdotH);

    // G�������߂�
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m�������߂�
    float m = PI * NdotV * NdotH;

    // �����܂ŋ��߂��A�l�𗘗p���āACook-Torrance���f���̋��ʔ��˂����߂�
    return max(F * D * G / m, 0.0);
}

/// <summary>
/// �t���l�����˂��l�������g�U���˂��v�Z
/// </summary>
/// <remark>
/// ���̊֐��̓t���l�����˂��l�������g�U���˗����v�Z���܂�
/// �t���l�����˂́A�������̂̕\�ʂŔ��˂��錻�ۂ̂Ƃ��ŁA���ʔ��˂̋����ɂȂ�܂�
/// ����g�U���˂́A�������̂̓����ɓ����āA�����������N�����āA�g�U���Ĕ��˂��Ă������̂��Ƃł�
/// �܂�t���l�����˂��ア�Ƃ��ɂ́A�g�U���˂��傫���Ȃ�A�t���l�����˂������Ƃ��́A�g�U���˂��������Ȃ�܂�
///
/// </remark>
/// <param name="N">�@��</param>
/// <param name="L">�����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B</param>
/// <param name="V">�����Ɍ������x�N�g���B</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // �@���ƌ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotNL = saturate(dot(N, L));

    // �@���Ǝ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotNV = saturate(dot(N, V));

    // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
    // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
    return (dotNL * dotNV);
}

float4 main(VSOutput In) : SV_Target0
{
    // �@�����擾
    float3 normal = In.Normal;
    
    // �A���x�h�J���[(�g�U���ˌ�)
    float4 albedoColor = g_albedoTex.Sample(g_ss, In.UV) * g_BaseColor;
    
    // �X�y�L�����J���[�̓A���x�h�Ɠ����ɂ���
    float3 specColor = albedoColor.rgb;
    
    // �����x
    float metallic = g_Metallic;
    
    // ���炩��
    float smooth = g_Roughness;
    
    // �����Ɍ������ĐL�т�x�N�g�����v�Z����
    float3 toEye = normalize(g_CamPos.xyz - In.wPos);
    
    float3 lig = 0;
    
    // �t���l�����˂��l�������f�B�Y�j�[�x�[�X�̊g�U���˂��v�Z
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -g_DL_Dir.rgb, toEye);

    //���K��Lambert�g�U���˂����߂�
    float NdotL = saturate(dot(normal, -g_DL_Dir.rgb));
    float3 lambertDiffuse = g_DL_Color.rgb * NdotL / PI;
    
    // �ŏI�I�Ȋg�U���ˌ����v�Z����
    float3 diffuse = albedoColor.rgb * diffuseFromFresnel * lambertDiffuse;

    // Cook-Torrance���f���̋��ʔ��˗����v�Z����
    float3 spec = CookTorranceSpecular(-g_DL_Dir.rgb, toEye, normal, smooth) * g_DL_Color.rgb;
    
    // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
    // �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);
    
    // ���炩����������΁A�g�U���˂͎キ�Ȃ�
    lig += diffuse * (1.0f - smooth) + spec;
    
    lig += g_Ambient_Color.rgb * albedoColor.rgb;
    
    return float4(lig, 1.0f);
}