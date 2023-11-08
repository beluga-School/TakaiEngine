#include "Ground.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv * tiling + offset);
    
    clip(m_alpha);

    float4 shadecolor = { 0, 0, 0, 1 };
    
    float threshold = 0.01f;
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            float3 ambient = texcolor.rgb * m_ambient * dirLights[i].lightcolor;
            
            float intensity = step(threshold, saturate(dot(normalize(input.normal), dirLights[i].lightv)));
            
            float3 difColor = texcolor.rgb;
            
            float3 diffuse = difColor * intensity * dirLights[i].lightcolor;
 
            shadecolor.rgb += (ambient + diffuse);
            shadecolor.a += 1;
        }
    }
    
    for (i = 0; i < POINTLIGHT_NUM; i++)
    {
        //ベクトルの長さ
        float distance = length(pointLights[i].lightpos - input.worldPos.xyz);
        float factor = pow(saturate(-distance / pointLights[i].radius + 1.0f),pointLights[i].decay);
        
        float3 pointColor = pointLights[i].lightcolor.rgb * pointLights[i].intensity * factor;
            
        float3 diffuse = pointColor * m_diffuse;
        
        float3 specular = pointColor * float3(1.0f,1.0f,1.0f);
            
        shadecolor.rgb += (diffuse + specular);
    }
    
    return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
}