#include "DitherTransparent.hlsli"

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

    int ditherUV_x = (int) fmod(input.svpos.x, 4.0f); //パターンの大きさで割ったときの余りを求める
    int ditherUV_y = (int) fmod(input.svpos.y, 4.0f); //今回のパターンサイズは4x4なので4で除算
    float dither = pattern[ditherUV_x + ditherUV_y * 4]; //求めた余りからパターン値を取得
    
    float camlength = magnitude(cameraPos - input.worldPos.xyz);
    
    float clamp_dither = saturate(dither / 4);
    float clamp_length = saturate(camlength / 50);
    
    clip(texcolor.w);
    
    clip(clamp_length - clamp_dither); //閾値が0以下なら描画しない

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