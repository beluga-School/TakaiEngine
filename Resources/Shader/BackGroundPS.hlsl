#include "BackGround.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv * tiling + offset);
    clip(texcolor.a);
    
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
    float3 outputColor = (texcolor.rgb * color.rgb) + float3(fogColor, fogColor, fogColor);
    
    //出力
    return float4(outputColor, color.a * texcolor.a);
}