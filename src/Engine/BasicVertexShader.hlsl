cbuffer worldMatrix : register(b0)
{
	float4x4 c_world;
}

cbuffer cameraBuffer : register(b3)
{
	float4x4 c_view;
	float4x4 c_proj;
    float4 c_camPos;
}

struct VSIn
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VSOut
{
	float4 pos : SV_POSITION;
	float2 uv: TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : WORLDPOS;
};

VSOut main(VSIn input)
{
	VSOut output;

	output.pos = float4(input.pos, 1.0f);
	output.pos = mul(c_world, output.pos);
    output.worldPos = output.pos.xyz;
	output.pos = mul(c_view, output.pos);
	output.pos = mul(c_proj, output.pos);

    output.normal = mul(c_world, float4(input.normal, 1.0f)).xyz;
    output.normal = normalize(output.normal);
	output.uv = input.uv;

	return output;
}