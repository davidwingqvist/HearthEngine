Texture2D t_color      : register(t0);
Texture2D t_normal     : register(t1);
Texture2D t_depth      : register(t2);
Texture2D t_worldPos   : register(t3);
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
    float4 tex = t_color.Sample(s_linearSamp, input.uv);
    float4 worldPos = t_worldPos.Sample(s_linearSamp, input.uv);
    float4 normal = t_normal.Sample(s_linearSamp, input.uv);
    float4 output = float4(0, 0, 0, 0);
    
    if(lightInfo.x == 0)
    {
        return tex;
    }
    
    float4 ambient_result = float4(0, 0, 0, 0);
    float4 diffuse_result = float4(0, 0, 0, 0);
    
    //float3 lightDirection = normalize(float3(0.0f, 1000.0f, 0.0f) - worldPos.xyz);
    float3 lightDirection = float3(0.1f, 0.0f, 0.1f);
    /*
        Loop through each light.
    */
    for (int i = 0; i < lightInfo.x; i++)
    {
        ambient_result = ambient[i] * tex;
        diffuse_result = max(dot(lightDirection, normalize(normal.xyz)), 0.1f) * tex;

    }
    
    ambient_result /= lightInfo.x;
    diffuse_result /= lightInfo.x;
    
    output = (ambient_result + diffuse_result) * tex;
    
    return output;
}