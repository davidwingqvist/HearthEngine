cbuffer worldMatrix : register(b0)
{
	float4x4 c_world;
}

cbuffer cameraBuffer : register(b1)
{
	float4x4 c_view;
	float4x4 c_proj;
}

struct VSIn
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VSOut
{
	float4 pos : SV_POSITION;
	float2 uv: TEXCOORD;
};

VSOut main(VSIn input)
{
	VSOut output;

	output.pos = float4(input.pos, 1.0f);
	output.pos = mul(c_world, output.pos);
	output.pos = mul(c_view, output.pos);
	output.pos = mul(c_proj, output.pos);

	output.uv = input.uv;

	return output;
}