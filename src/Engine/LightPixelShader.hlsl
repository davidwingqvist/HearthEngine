Texture2D t_color      : register(t0);
Texture2D t_normal     : register(t1);
Texture2D t_depth      : register(t2);
SamplerState s_linearSamp : register(s0);
cbuffer lightData : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 data; // Specific data for light types.
    float type;
}

cbuffer lightInfo : register(b1)
{
    float4 lightInfo;
}

struct PxIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PxIn input) : SV_TARGET
{
    float4 output = t_color.Sample(s_linearSamp, input.uv);
    
    int test = 0;
    /*
        Loop through each light.
    */
    for (int i = 0; i < lightInfo.x; i++)
    {
        
    }
    
    return output;
}