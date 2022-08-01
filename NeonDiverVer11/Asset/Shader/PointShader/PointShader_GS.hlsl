#include "inc_PointShader.hlsli"

static const uint vnum = 4;

static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0),
	float4(-0.5f, 0.5f, 0, 0),
	float4(0.5f, -0.5f, 0, 0),
	float4(0.5f, 0.5f, 0, 0)
};

static const float2 uv_offsetArray[vnum] =
{
    float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0)
};

[maxvertexcount(vnum)]
void main(
	point GSInput In[1],
	inout TriangleStream<PSInput> Out
)
{
    row_major matrix mScale = CreateScale(particleData[In[0].index].scale);
    row_major matrix mRotate = CreateRotatinZ(particleData[In[0].index].rotate);

    float4 ParticlePos = mul(float4(particleData[In[0].index].pos,1), g_mWorld);
    ParticlePos.xyz += GetPosition(g_mWorld);
    
    row_major matrix mParticle = g_mBillboard;
    mParticle._41_42_43_44 = ParticlePos;
    mParticle = mul(mScale, mul(mRotate, mParticle));
    
    for (uint i = 0; i < vnum; i++)
    {
        PSInput element;
		
        element.Pos = In[0].Pos + offset_array[i];
        element.Pos = mul(element.Pos, mParticle);
        element.Pos = mul(element.Pos, g_mVP);
        element.index = In[0].index;
        element.UV = uv_offsetArray[i];
        Out.Append(element);
    }
}