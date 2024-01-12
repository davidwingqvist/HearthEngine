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
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.normals = float4(input.normal, 1.0f);
    return output;
}