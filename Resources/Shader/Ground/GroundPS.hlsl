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
           
            shadecolor.rgb -= diffuse + specular;
        }
    }
    
    for (int k = 0; k < SPOTLIGHT_NUM; k++)
    {
        if (spotLights[k].active)
        {
            float3 spotlightDirectionOnSurface = normalize(input.worldPos.xyz - spotLights[k].lightpos);
            float cosAngle = dot(spotlightDirectionOnSurface, spotLights[k].direction);
            float falloffFactor = saturate((cosAngle - spotLights[k].cosAngle) / (1.0f - spotLights[k].cosAngle));
            
            float distance = length(spotLights[k].lightpos - input.worldPos.xyz);
            float attenuationFactor = pow(saturate(-distance / spotLights[k].distance + 1.0f), spotLights[k].decay);
            
            float3 spotColor = spotLights[k].lightcolor.rgb * spotLights[k].intensity * falloffFactor * attenuationFactor;
            
            float3 diffuse = spotColor * m_diffuse;
        
            float3 specular = spotColor * float3(1.0f, 1.0f, 1.0f);
        
            shadecolor.rgb += diffuse + specular;
        }
    }
    
    for (i = 0; i < CIRCLESHADOW_NUM; i++)
    {
        if (circleShadow[i].active)
        {
            float3 casterVec = circleShadow[i].casterPos - input.worldPos.xyz;
            
            float distance = dot(casterVec, circleShadow[i].direction);
            
            float atten = saturate(1.0f / (circleShadow[i].atten.x + circleShadow[i].atten.y * distance + circleShadow[i].atten.z * distance * distance));

            atten *= step(0, distance);
            
            float3 lightpos = circleShadow[i].casterPos + circleShadow[i].direction * circleShadow[i].distance;
        
            float lightVec = normalize(lightpos - input.worldPos.xyz);
            
            float cos = dot(lightVec, circleShadow[i].direction);
            
            float angleAtten = smoothstep(circleShadow[i].angleCos.y, circleShadow[i].angleCos.x, cos);
        
            atten *= angleAtten;
            
            shadecolor.rgb -= atten;
        }
    }
    
    return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
}