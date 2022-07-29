cbuffer cbCamera : register(b0)
{
    row_major matrix g_mView;
    row_major matrix g_mProj;
    row_major matrix g_mVP;
    row_major matrix g_mBillboard;
    float3 g_CamPos;
}

cbuffer cbDirectionLight : register(b1)
{
    float4 g_DL_Dir;
    float4 g_DL_Color;
    float4 g_Ambient_Color;
}