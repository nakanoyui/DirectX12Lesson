struct VSOutput
{
    float4 Pos : SV_Position;
    float4 Color : Color;
};

cbuffer cbWorld : register(b2)
{
    row_major matrix g_mWorld;
}