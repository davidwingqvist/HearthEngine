Texture2D t_color      : register(t0);
Texture2D t_normal     : register(t1);
Texture2D t_depth      : register(t2);
SamplerState s_linearSamp : register(s0);

struct PxIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PxIn input) : SV_TARGET
{
    float4 output = t_color.Sample(s_linearSamp, input.uv);
    return output;
}