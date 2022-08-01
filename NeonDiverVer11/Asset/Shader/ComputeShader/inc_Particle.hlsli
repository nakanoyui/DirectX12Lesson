// ƒ‰ƒ“ƒ_ƒ€ŠÖ”
float3 GetRandomVector(float x)
{
    float3 returnData = float3(0, 0, 0);
    returnData.x = frac(sin(dot(float2(0.016f * x, 0), float2(12.9898, 78.233))) * 43758.5453);
    returnData.y = frac(sin(dot(float2(0, 0.016f * x), float2(12.9898, 78.233))) * 43758.5453);
    returnData.z = frac(sin(dot(float2(0.016f * x, 0.016f * x), float2(12.9898, 78.233))) * 43758.5453);
    return returnData;
}

float3 GetRandomVector(float3 vec)
{
    float3 result = float3(0, 0, 0);
    result.x = frac(sin(dot(vec.zy + vec.x, float2(12.9898, 78.233))) * 43758.5453);
    result.y = frac(sin(dot(vec.yz + vec.x, float2(12.9898, 78.233))) * 43758.5453);
    result.z = frac(sin(dot(vec.xy + vec.z, float2(12.9898, 78.233))) * 43758.5453);
    return result;
}

// ƒ‰ƒ“ƒ_ƒ€ŠÖ””ÍˆÍw’è”Å
float3 GetRandomVector(float3 vec, float3 minValue, float3 maxValue)
{
    float3 result = float3(0, 0, 0);
    result = GetRandomVector(vec);
    result = (result / 1.0f * maxValue) - (-minValue) / 2.0f;
    return result;
}