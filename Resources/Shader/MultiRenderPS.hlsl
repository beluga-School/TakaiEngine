#include "Phong.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input)
{
    PSOutput output;
    
    float4 texcolor = tex.Sample(smp, input.uv);
    
    //光沢度
    const float shininess = 4.0f;
    
    float3 eyedir = normalize(cameraPos - input.worldPos.xyz);
    float3 ambient = m_ambient;
    
    //シェーディングによる色
    float4 shadecolor = float4(ambientColor * ambient,m_alpha);
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if(dirLights[i].active)
        {
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
            
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
  
            float3 diffuse = dotlightnormal * m_diffuse;
    
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
        }
    }
    for (i = 0; i < POINTLIGHT_NUM; i++)
    {
        if (pointLights[i].active)
        {
            //ライトへのベクトル
            float3 lightv = pointLights[i].lightpos - input.worldPos.xyz;
            lightv = normalize(lightv);
            //ベクトルの長さ
            float d = length(lightv);
            //距離減衰係数
            float atten = 1.0f / (
            pointLights[i].lightatten.x +
            pointLights[i].lightatten.y * d +
            pointLights[i].lightatten.z * d * d);
            
            float3 dotlightnormal = dot(lightv, input.normal);
            
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
  
            float3 diffuse = dotlightnormal * m_diffuse;
    
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
            
        }
    }
        
    output.target0 = shadecolor * texcolor * color;
    output.target1 = float4(1 - (shadecolor * texcolor * color).rgb,1);
    
    return output;
}