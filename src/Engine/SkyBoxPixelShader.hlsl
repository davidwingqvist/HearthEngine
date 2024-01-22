TextureCube cubemap : register(t0);

SamplerState samp : register(s0);

struct PixelIn
{
    float3 uv : TEXCOORD;
    float4 pos : SV_Position;
};

float4 main(PixelIn input) : SV_TARGET
{
    return cubemap.Sample(samp, input.uv) * 0.85f;

}