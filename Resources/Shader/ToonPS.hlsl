#include "Toon.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー


float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
    //光沢度
    const float shininess = 4.0f;
    
    //前の処理　多分入れる値をちゃんと理解したらこっちが正しい
    //float3 eyedir = normalize(cameraPos - input.worldPos.xyz);
    
    //cameraPos参照だと、やりたいことと噛み合ってなかったので勝手に太陽の位置を決定
    float3 solPos = { 0, 100, 0 };
    float3 eyedir = normalize(solPos - input.worldPos.xyz);
    float3 ambient = m_ambient;
    //float3 ambient = float3(0.0f,0.0f,1.0f);
    
    float t = 0.01f;
    
    //シェーディングによる色
    float3 ambientColor = float3(0.8f, 0.8f, 0.8f);
    
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
            
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
            
            //トゥーンはここをいじらないといけない
            float3 diffuse = step(t,dotlightnormal) * float3(0.1f,0.1f,0.1f);
            
            float3 specular = step(0.2f,pow(saturate(dot(reflect, eyedir)), shininess)) * m_specular;
            
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
            //shadecolor.rgb += diffuse * dirLights[i].lightcolor;
        }
    }
    //ポイントライトは使ってないのでいったん消す

    return shadecolor * texcolor * color;
}