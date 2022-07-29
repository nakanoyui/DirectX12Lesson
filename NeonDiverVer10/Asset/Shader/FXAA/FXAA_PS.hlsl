#include "inc_FXAA.hlsli"

Texture2D<float4> g_inputTex : register(t0); // ���̓e�N�X�`��
SamplerState g_ss : register(s1); // �T���v��

float4 FxaaPixelShader(
	float2 pos,
    float4 fxaaConsoleRcpFrameOpt,
    float4 fxaaConsoleRcpFrameOpt2,
    float fxaaQualityEdgeThreshold,
    float fxaaQualityEdgeThresholdMin,
    float fxaaConsoleEdgeSharpness,
    float fxaaConsoleEdgeThreshold,
    float fxaaConsoleEdgeThresholdMin,
    float2 sceneTexSize
)
{
	// �ߖT4�e�N�Z�����t�F�b�`�B
    float4 nTex = float4(
		pos.x + 0.5f / sceneTexSize.x,
		pos.y + 0.5f / sceneTexSize.y,
		pos.x - 0.5f / sceneTexSize.x,
		pos.y - 0.5f / sceneTexSize.y
	);
    
    float lumaNw = g_inputTex.Sample(g_ss, nTex.xy).y;
    float lumaSw = g_inputTex.Sample(g_ss, nTex.xw).y;
    float lumaNe = g_inputTex.Sample(g_ss, nTex.zy).y;
    float lumaSe = g_inputTex.Sample(g_ss, nTex.zw).y;
	
    float4 rgbyM = g_inputTex.Sample(g_ss, pos.xy);
    float lumaM = rgbyM.y;
	
	
	// �ߖT�e�N�Z���̋P�x�̍��𒲂ׂ�B
    float lumaMaxNwSw = max(lumaNw, lumaSw);
    lumaNe += 1.0 / 384.0;
    float lumaMinNwSw = min(lumaNw, lumaSw);
/*--------------------------------------------------------------------------*/
    float lumaMaxNeSe = max(lumaNe, lumaSe);
    float lumaMinNeSe = min(lumaNe, lumaSe);
/*--------------------------------------------------------------------------*/
    float lumaMax = max(lumaMaxNeSe, lumaMaxNwSw);
    float lumaMin = min(lumaMinNeSe, lumaMinNwSw);
/*--------------------------------------------------------------------------*/
    float lumaMaxScaled = lumaMax * fxaaConsoleEdgeThreshold;
/*--------------------------------------------------------------------------*/
    float lumaMinM = min(lumaMin, lumaM);
    float lumaMaxScaledClamped = max(fxaaConsoleEdgeThresholdMin, lumaMaxScaled);
    float lumaMaxM = max(lumaMax, lumaM);
    float dirSwMinusNe = lumaSw - lumaNe;
    float lumaMaxSubMinM = lumaMaxM - lumaMinM;
    float dirSeMinusNw = lumaSe - lumaNw;
    if (lumaMaxSubMinM < lumaMaxScaledClamped)
    {
		// �P�x�̍����������l�ȉ��������̂ŁA���̃s�N�Z���̓A���`�������Ȃ��B
        return rgbyM;
    }
    
/*--------------------------------------------------------------------------*/
	// �P�x�̍��𗘗p���āA�M�U�M�U���������Ă���\���̍����e�N�Z�����t�F�b�`����B
    float2 dir;
    dir.x = dirSwMinusNe + dirSeMinusNw;
    dir.y = dirSwMinusNe - dirSeMinusNw;
   
/*--------------------------------------------------------------------------*/
    float2 dir1 = normalize(dir.xy);
   
    float4 rgbyN1 = g_inputTex.Sample(g_ss, pos.xy - dir1 * fxaaConsoleRcpFrameOpt.zw);
    float4 rgbyP1 = g_inputTex.Sample(g_ss, pos.xy + dir1 * fxaaConsoleRcpFrameOpt.zw);
/*--------------------------------------------------------------------------*/
    float dirAbsMinTimesC = min(abs(dir1.x), abs(dir1.y)) * fxaaConsoleEdgeSharpness;
    float2 dir2 = clamp(dir1.xy / dirAbsMinTimesC, -2.0, 2.0);
/*--------------------------------------------------------------------------*/
    float4 rgbyN2 = g_inputTex.Sample(g_ss, pos.xy - dir2 * fxaaConsoleRcpFrameOpt2.zw);
    float4 rgbyP2 = g_inputTex.Sample(g_ss, pos.xy + dir2 * fxaaConsoleRcpFrameOpt2.zw);
    
/*--------------------------------------------------------------------------*/
	// �u�����h�u�����h
    float4 rgbyA = rgbyN1 + rgbyP1;
    float4 rgbyB = ((rgbyN2 + rgbyP2) * 0.25) + (rgbyA * 0.25);
/*--------------------------------------------------------------------------*/
    int twoTap = (rgbyB.y < lumaMin) || (rgbyB.y > lumaMax);
    
    if (twoTap)
    {
		// �܂��P�x�̍����傫���̂ŁA�ēx�u�����h�B
        rgbyB.xyz = rgbyA.xyz * 0.5;
    }
    return rgbyB;
    
}

float4 main(VSOutput In) : SV_Target0
{
    float2 texSize = float2(1280, 720); // �����ƂŒ萔�o�b�t�@�[�œn��
    float4 rcpFrame = float4(0.0f, 0.0f, 1.0f / texSize.x, 1.0f / texSize.y);

    float4 srcColor = g_inputTex.Sample(g_ss,In.UV);
    
    float4 color = FxaaPixelShader(
        In.UV,
        rcpFrame,
        rcpFrame,
        0.166f,
        0.0833f,
        1.0f,
        0.4f,
        0.0833f,
        texSize
    );
    
    return color;
}