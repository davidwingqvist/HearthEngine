#include "Functions.hlsli"

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

cbuffer lightWorld : register(b2)
{
    float4 c_lightPosition;
}

cbuffer cameraBuffer : register(b3)
{
    float4x4 c_view;
    float4x4 c_proj;
    float4 c_camPos;
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
    float4 specular_result = float4(0, 0, 0, 0);
    

    /*
        Loop through each light.
    */
    for (int i = 0; i < lightInfo.x; i++)
    {
        float3 lightDirection = normalize(c_lightPosition.xyz - worldPos.xyz);
        ambient_result += ambient[i] * tex;
        diffuse_result += max(dot(lightDirection, normalize(normal.xyz)), 0.0f) * tex;
        
        float3 viewDirection = normalize(c_camPos.xyz - worldPos.xyz);
        float3 reflection = reflect(lightDirection, normal.xyz);
        specular_result += pow(max(dot(viewDirection, reflection), 0.0f), 32);
    }
    
    ambient_result /= lightInfo.x;
    diffuse_result /= lightInfo.x;
    specular_result /= lightInfo.x;
    
    output = (ambient_result + diffuse_result + specular_result) * tex;
    
    return output;
}