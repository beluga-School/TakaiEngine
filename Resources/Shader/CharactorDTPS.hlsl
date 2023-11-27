#include "CharactorDT.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float> depthTex : register(t1); //1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);

static const int pattern[16] =
{
    0, 8, 2, 10,
    12, 4, 14, 6,
    3, 11, 1, 9,
    15, 7, 13, 5
};

float3 magnitude(float3 v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv * tiling + offset);
    float maskTexcolor = depthTex.Sample(smp, input.uv * tiling + offset);
    
    int ditherUV_x = (int) fmod(input.svpos.x, 4.0f); //パターンの大きさで割ったときの余りを求める
    int ditherUV_y = (int) fmod(input.svpos.y, 4.0f); //今回のパターンサイズは4x4なので4で除算
    float dither = pattern[ditherUV_x + ditherUV_y * 4]; //求めた余りからパターン値を取得
    
    float camlength = magnitude(cameraPos - input.worldPos.xyz);
    
    float clamp_dither = saturate(dither / 16);
    float clamp_length = saturate(camlength / 50);
    
    clip(clamp_length - clamp_dither); //閾値が0以下なら描画しない

    float4 shadecolor = { 0, 0, 0, 1 };
    
    float threshold = 0.01f;
       
    //スペキュラーしきい値
    float speThreshold = 0.2f;
    
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
            
            //スペキュラー
            //視線ベクトル
            float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
            //入射光
            float3 lightDir = normalize(dirLights[i].lightv);
            
            float3 normalizeVec = normalize(input.normal);
            //反射光
            float3 reflectDir = -lightDir + 2 * normalizeVec * dot(normalizeVec, lightDir);
            
            float3 specular = step(speThreshold, pow(saturate(dot(reflectDir, eyeDir)), 20) * dirLights[i].lightcolor);
            
            shadecolor.rgb += (ambient + diffuse + specular);
            shadecolor.a += 1;
        }
    }
    
    return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
    //return float4(maskTexcolor, 0, 0, 1);
}