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
 
            //shadecolor.rgb += (ambient + diffuse);
            //shadecolor.a += 1;
        }
    }
    
    for (i = 0; i < POINTLIGHT_NUM; i++)
    {
        //ライトへのベクトル

        float3 lightv = pointLights[i].lightpos - input.worldPos.xyz;
        lightv = normalize(lightv);
        //ベクトルの長さ
        float d = length(lightv);
        
        float3 pointColor = pointLights[i].lightcolor.rgb * pointLights[i].intensity;
        
        float3 dotlightnormal = dot(lightv, input.normal);
            
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
  
        float3 diffuse = pointColor * dotlightnormal * m_diffuse;
    
        //float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
        
        float3 specular = pointColor * m_specular;
            
        shadecolor.rgb += (diffuse + specular);
    }
    
    return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
}