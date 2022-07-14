cbuffer cbCamera : register(b0)
{
    row_major matrix g_mView;
    row_major matrix g_mProj;
    float4 g_CamPos;
    row_major matrix g_mBillboard;
}

cbuffer cbDirectionLight : register(b1)
{
    float4 g_DL_Dir;
    float4 g_DL_Color;
    float4 g_Ambient_Color;
}