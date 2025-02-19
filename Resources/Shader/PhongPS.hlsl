#include "Phong.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
    float4 shadecolor = { 0, 0, 0, 1 };
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            
            
            //アンビエント = オブジェクトの元の色 * 暗くする率 * 光の色
            float3 ambient = texcolor.rgb * m_ambient * dirLights[i].lightcolor;
            
            //ディフューズ
            //光との内積値
            float intensity = saturate(dot(normalize(input.normal), dirLights[i].lightv));
            //オブジェクトの色
            float3 difColor = texcolor.rgb;
            
            //ディフューズ = オブジェクトの元の色 * 光との内積値 * 光の色
            float3 diffuse = difColor * intensity * dirLights[i].lightcolor;
            
            //スペキュラー
            //視線ベクトル
            float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
            //入射光
            float3 lightDir = normalize(dirLights[i].lightv);
            
            float3 normalizeVec = normalize(input.normal);
            //反射光
            float3 reflectDir = -lightDir + 2 * normalizeVec * dot(normalizeVec, lightDir);
            
            float3 specular = pow(saturate(dot(reflectDir, eyeDir)), 20) * dirLights[i].lightcolor;
            
            shadecolor.rgb += (ambient + diffuse + specular);
            shadecolor.a += 1;
        }
    }

    return shadecolor * color;
    
    //for (i = 0; i < POINTLIGHT_NUM; i++)
    //{
    //    if (pointLights[i].active)
    //    {
    //        //ライトへのベクトル
    //        float3 lightv = pointLights[i].lightpos - input.worldPos.xyz;
    //        lightv = normalize(lightv);
    //        //ベクトルの長さ
    //        float d = length(lightv);
    //        //距離減衰係数
    //        float atten = 1.0f / (
    //        pointLights[i].lightatten.x +
    //        pointLights[i].lightatten.y * d +
    //        pointLights[i].lightatten.z * d * d);
            
    //        float3 dotlightnormal = dot(lightv, input.normal);
            
    //        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
  
    //        float3 diffuse = dotlightnormal * m_diffuse;
    
    //        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
    //        shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
            
    //    }
    //}
}