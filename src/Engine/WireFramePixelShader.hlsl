cbuffer buff : register(b0)
{
    float4 color;
};

float4 main() : SV_Target0
{
	// Paint wireframe
	return color;
}