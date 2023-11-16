#include "Ground.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    
    float4 texcolor = tex.Sample(smp, input.uv * tiling + offset);
    
    clip(m_alpha);
    //clip(texcolor.r + texcolor.g + texcolor.b);
    
    //float radius_ = 1;
    //float dist = distance(input.worldPos.xyz, cameraPos);
    //float clamp_distance = saturate(dist / radius_);
    
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
    
    return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
}