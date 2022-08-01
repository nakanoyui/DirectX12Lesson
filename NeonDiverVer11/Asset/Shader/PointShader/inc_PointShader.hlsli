cbuffer cbWorld : register(b0)
{
    row_major matrix g_mWorld;
}

cbuffer cbCamera : register(b1)
{
    row_major matrix g_mView;
    row_major matrix g_mProj;
    row_major matrix g_mVP;
    row_major matrix g_mBillboard;
    float4 g_CamPos;
}

struct GSInput
{
    float4 Pos : SV_POSITION;
    uint index : TEXCOORD;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    uint index : TEXCOORD;
    float2 UV : TEXCOORD1;
};

struct ParticleData
{
    int index;
    int state; // 状態
    float life; // ライフ

	// 基礎データ
    float3 pos;
    float3 scale;
    float rotate;
    float3 vel;
    float3 col;
};

StructuredBuffer<ParticleData> particleData : register(t0);

float3 GetPosition(matrix mat)
{
    return float3(mat._41, mat._42, mat._43);
}

row_major matrix CreateTranslation(float3 position)
{
    row_major matrix Out =
    {
        1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		position.x, position.y, position.z, 1,
    };
    
    return Out;
}

row_major matrix CreateScale(float3 scale)
{
    row_major matrix Out =
    {
        scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1,
    };
    
    return Out;
}

row_major matrix CreateRotatinX(float x)
{
    row_major matrix Out =
    {
        1, 0, 0, 0,
		0, cos(x), sin(x), 0,
		0, -sin(x), cos(x), 0,
		0, 0, 0, 1,
    };
    
    return Out;
}

row_major matrix CreateRotatinY(float y)
{
    row_major matrix Out =
    {
        cos(y), 0, -sin(y), 0,
		0, 1, 0, 0,
	    sin(y), 0, cos(y), 0,
		0, 0, 0, 1,
    };
    
    return Out;
}

row_major matrix CreateRotatinZ(float z)
{
    row_major matrix Out =
    {
        cos(z), sin(z), 0, 0,
		-sin(z), cos(z), 0, 0,
	    0, 1, 0, 0,
		0, 0, 0, 1,
    };
    
    return Out;
}