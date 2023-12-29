#include "Ground.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float3 magnitude(float3 v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

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
            //ベクトルの長さ
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
   
            shadecolor.rgb -= atten * float3(0.1f, 0.1f, 0.1f);
        }
    }
    
    //カメラと頂点の距離を計算
    float3 camLength = (cameraPos - input.worldPos.xyz);
    camLength = abs(camLength);
    
    //フォグが完全にかかる距離
    const float FOG_MAX = 1000.0f;
    //フォグがかかり始める距離
    const float FOG_START = 50.f;
    
    //フォグ色の強さを算出
    float fogWeight = (camLength.x + camLength.z) / 2 - FOG_START;
    fogWeight = max(fogWeight, 0);
    
    //フォグ色の強さをかかる距離で割って小さめの値に変換(ここら辺適当。きちんとやるなら0~1の値にしっかり収まるようにしたい)
    float fogColor = fogWeight / FOG_MAX;

    //最終的な色を算出(丸影とかの色を出した値に無理やり乗せる形なので、何かしら問題が起きそう。こわい。)
    float3 outputColor = (shadecolor.rgb * color.rgb) + float3(fogColor, fogColor, fogColor);
    
    //出力
    return float4(outputColor, color.a * texcolor.a);
    //return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
}