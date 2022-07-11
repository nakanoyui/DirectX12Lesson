#include "inc_StandardShader.hlsli"
#include "../inc_Common.hlsli"

Texture2D g_albedoTex : register(t0); // アルベドテクスチャ
Texture2D g_normalMap : register(t1); // 法線マップ
Texture2D g_metalRoughMap : register(t2); // メタリック/ラフネステクスチャ
Texture2D g_emissiveMap : register(t3); // 発光テクスチャ

SamplerState g_ss : register(s0); // サンプラ

static const float PI = 3.1415926f; // π

float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normalMap.Sample(g_ss, uv).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

    return newNormal;
}

// ベックマン分布を計算する
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

// フレネルを計算。Schlick近似を使用
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// Cook-Torranceモデルの鏡面反射を計算
/// </summary>
/// <param name="L">光源に向かうベクトル</param>
/// <param name="V">視点に向かうベクトル</param>
/// <param name="N">法線ベクトル</param>
/// <param name="metallic">金属度</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic)
{
    float microfacet = 0.76f;

    // 金属度を垂直入射の時のフレネル反射率として扱う
    // 金属度が高いほどフレネル反射は大きくなる
    float f0 = metallic;

    // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    // 各種ベクトルがどれくらい似ているかを内積を利用して求める
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D項をベックマン分布を用いて計算する
    float D = Beckmann(microfacet, NdotH);

    // F項をSchlick近似を用いて計算する
    float F = SpcFresnel(f0, VdotH);

    // G項を求める
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m項を求める
    float m = PI * NdotV * NdotH;

    // ここまで求めた、値を利用して、Cook-Torranceモデルの鏡面反射を求める
    return max(F * D * G / m, 0.0);
}

/// <summary>
/// フレネル反射を考慮した拡散反射を計算
/// </summary>
/// <remark>
/// この関数はフレネル反射を考慮した拡散反射率を計算します
/// フレネル反射は、光が物体の表面で反射する現象のとこで、鏡面反射の強さになります
/// 一方拡散反射は、光が物体の内部に入って、内部錯乱を起こして、拡散して反射してきた光のことです
/// つまりフレネル反射が弱いときには、拡散反射が大きくなり、フレネル反射が強いときは、拡散反射が小さくなります
///
/// </remark>
/// <param name="N">法線</param>
/// <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
/// <param name="V">視線に向かうベクトル。</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // 法線と光源に向かうベクトルがどれだけ似ているかを内積で求める
    float dotNL = saturate(dot(N, L));

    // 法線と視線に向かうベクトルがどれだけ似ているかを内積で求める
    float dotNV = saturate(dot(N, V));

    // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
    // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
    return (dotNL * dotNV);
}

float4 main(VSOutput In) : SV_Target0
{
    // 法線を取得
    float3 normal = In.Normal;
    
    // アルベドカラー(拡散反射光)
    float4 albedoColor = g_albedoTex.Sample(g_ss, In.UV) * g_BaseColor;
    
    // スペキュラカラーはアルベドと同じにする
    float3 specColor = albedoColor.rgb;
    
    // 金属度
    float metallic = g_Metallic;
    
    // 滑らかさ
    float smooth = g_Roughness;
    
    // 視線に向かって伸びるベクトルを計算する
    float3 toEye = normalize(g_CamPos.xyz - In.wPos);
    
    float3 lig = 0;
    
    // フレネル反射を考慮したディズニーベースの拡散反射を計算
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -g_DL_Dir.rgb, toEye);

    //正規化Lambert拡散反射を求める
    float NdotL = saturate(dot(normal, -g_DL_Dir.rgb));
    float3 lambertDiffuse = g_DL_Color.rgb * NdotL / PI;
    
    // 最終的な拡散反射光を計算する
    float3 diffuse = albedoColor.rgb * diffuseFromFresnel * lambertDiffuse;

    // Cook-Torranceモデルの鏡面反射率を計算する
    float3 spec = CookTorranceSpecular(-g_DL_Dir.rgb, toEye, normal, smooth) * g_DL_Color.rgb;
    
    // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
    // スペキュラカラーの強さを鏡面反射率として扱う
    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);
    
    // 滑らかさが高ければ、拡散反射は弱くなる
    lig += diffuse * (1.0f - smooth) + spec;
    
    lig += g_Ambient_Color.rgb * albedoColor.rgb;
    
    return float4(lig, 1.0f);
}