#include "SingleColor.hlsli"


Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}