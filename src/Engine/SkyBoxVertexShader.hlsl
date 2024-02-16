cbuffer cameraBuffer : register(b3)
{
    float4x4 c_view;
    float4x4 c_proj;
}

struct VertexOut
{
    float3 uv : TEXCOORD;
    float4 pos : SV_Position;
};

VertexOut main( float3 pos : SKYPOS )
{
    VertexOut output;
    
    output.uv = pos;
    
    float4x4 mat = mul(c_proj, c_view);
    
    output.pos = mul(mat, float4(pos, 0.0f)).xyww;
    
	return output;
}