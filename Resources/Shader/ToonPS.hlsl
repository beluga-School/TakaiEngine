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
    float3 solDir = normalize(solPos - input.worldPos.xyz);
    float3 ambient = m_ambient;
    //float3 ambient = float3(0.0f,0.0f,1.0f);
    
    //視点ベクトル
    float3 eyeDir = normalize(input.worldPos.xyz - cameraPos);
    
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
            float3 diffuse = smoothstep(t, t + 0.05f, dotlightnormal) * float3(0.1f, 0.1f, 0.1f);
            
            //太陽光でスペキュラーが動かないように(なんか間違ってない？)
            float3 specular = smoothstep(0.15f, 0.2f, pow(saturate(dot(reflect, eyeDir)), shininess)) * m_specular;
            
           
            //shadecolor.rgb += diffuse * dirLights[i].lightcolor;
           
            
            
            //加算
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
          
            
            ////リムライト
            //float rimPower = 2.0f;
                    
            //float3 eye_vector = normalize(-abs(cameraDir));
            //float3 light_vector = normalize(dirLights[i].lightv);
            //float3 normal = normalize(input.normal);
            
            //float rim = 1.0f - dot(normal, eye_vector);
            
            //rim = pow(abs(rim), rimPower);
            
            //float light_rim = max(dot(-light_vector, eye_vector), 0.0f);
            
            //float3 rimcolor = rim * light_rim;
            ////リムライトはわけて加算してみる
            //shadecolor.rgb += rimcolor;
            
            //ポイントライトは使ってないのでいったん消す
        }
    }
    
    

    return shadecolor * texcolor * color;
}