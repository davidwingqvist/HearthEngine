sampler samp : register(s0);
Texture2D albedo : register(s0);

struct PxIn
{
	float4 position : SV_POSITION;
	float2 uv: TEXCOORD;
    float3 normal : NORMAL;
};

struct PxOut
{
    float4 color : SV_Target0;
	float4 normals : SV_Target1;
};

PxOut main(PxIn input)
{
    PxOut output;
    output.color = albedo.Sample(samp, input.uv);
    output.normals = float4(input.normal, 1.0f);
    return output;
}