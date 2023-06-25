#include "NonePostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = tex0.Sample(smp, input.uv);
    
    return float4(color.rgb, 1);
}