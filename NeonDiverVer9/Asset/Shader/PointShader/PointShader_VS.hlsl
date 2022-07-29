#include "inc_PointShader.hlsli"

GSInput main(float4 pos : POSITION, uint index : SV_InstanceID)
{
    GSInput Out;
    Out.Pos = pos;
    Out.index = index;
    return Out;
}