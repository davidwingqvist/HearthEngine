Texture2D t_pos : register(t0);
Texture2D t_normal : register(t1);

struct PxIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PxIn input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}