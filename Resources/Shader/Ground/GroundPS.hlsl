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
    
    for (int j = 0; j < POINTLIGHT_NUM; j++)
    {
        if (pointLights[j].active)
        {
            //ƒxƒNƒgƒ‹‚Ì’·‚³
            float distance = length(pointLights[j].lightpos - input.worldPos.xyz);
            float factor = pow(saturate(-distance / pointLights[j].radius + 1.0f), pointLights[j].decay);
            
            float3 pointColor = pointLights[j].lightcolor.rgb * pointLights[j].intensity * factor;
            
            float3 diffuse = pointColor * m_diffuse;
        
            float3 specular = pointColor * float3(1.0f, 1.0f, 1.0f);
           
            shadecolor.rgb += diffuse + specular;
        }
    }
    
    for (i = 0; i < CIRCLESHADOW_NUM; i++)
    {
        if (circleShadows[i].active)
        {
            float3 casterv = circleShadows[i].casterPos - input.worldPos.xyz;
            
            float d = dot(casterv, circleShadows[i].direction);
            
            float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
            
            atten *= step(0, d);
            
            float3 lightpos = circleShadows[i].casterPos + circleShadows[i].direction * circleShadows[i].distance;
            
            float3 lightv = normalize(lightpos - input.worldPos.xyz);
            
            float cos = dot(lightv, circleShadows[i].direction);
            
            float angleatten = smoothstep(circleShadows[i].cosAngle.y, circleShadows[i].cosAngle.x, cos);
            
            atten *= angleatten;
   
            shadecolor.rgb -= atten * float3(0.1f,0.1f,0.1f);
        }
    }
    
    return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
}