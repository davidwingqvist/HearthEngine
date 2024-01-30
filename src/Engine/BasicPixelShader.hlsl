sampler samp : register(s0);
Texture2D albedo : register(s0);

struct PxIn
{
	float4 position : SV_POSITION;
	float2 uv: TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : WORLDPOS;
};

struct PxOut
{
    float4 color : SV_Target0;
	float4 normals : SV_Target1;
    float4 worldPos : SV_Target2;
};

PxOut main(PxIn input)
{
    PxOut output;
    output.color = albedo.Sample(samp, input.uv);
    output.normals = float4(input.normal, 1.0f);
    output.worldPos = float4(input.worldPos, 1.0f);
    return output;
}