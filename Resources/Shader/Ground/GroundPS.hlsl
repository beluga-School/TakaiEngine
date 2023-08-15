#include "Ground.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv * tiling);
    
    float4 shadecolor = { 0, 0, 0, 1 };
    
    //しきい値
    float threshold = 0.01f;
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            //アンビエント = オブジェクトの元の色 * 暗くする率 * 光の色
            float3 ambient = texcolor.rgb * m_ambient * dirLights[i].lightcolor;
            
            //ディフューズ
            //光との内積値
            float intensity = step(threshold, saturate(dot(normalize(input.normal), dirLights[i].lightv)));
            //オブジェクトの色
            float3 difColor = texcolor.rgb;
            
            //ディフューズ = オブジェクトの元の色 * 光との内積値 * 光の色
            float3 diffuse = difColor * intensity * dirLights[i].lightcolor;

            //地面はスペキュラーいらないので削除
            
            shadecolor.rgb += (ambient + diffuse);
            shadecolor.a += 1;
        }
    }
    
    return shadecolor * color;
}