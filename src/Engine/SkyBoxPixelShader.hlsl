TextureCube cubemap : register(t0);

SamplerState samp : register(s0);

struct PixelIn
{
    float3 uv : TEXCOORD;
    float4 pos : SV_Position;
};

float4 main(PixelIn input) : SV_TARGET
{
    return lerp(float4(0.2f, 0.2f, 0.2f, 0.0f), cubemap.Sample(samp, input.uv), 0.2f);

}