cbuffer cameraBuffer : register(b3)
{
    float4x4 c_view;
    float4x4 c_proj;
}

// Pass through vertex shader
float4 main(float4 pos : POSITION) : SV_POSITION
{
    pos.w = 1.0f;
    float4 output = mul(c_view, pos);
    output = mul(c_proj, output);
    return output;
}