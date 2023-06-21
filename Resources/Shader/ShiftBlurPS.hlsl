#include "ShiftBlur.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = {0,0,0,0};
	
    const float shiftWidth = 0.005f;
    const float shiftNum = 3.0f;
    
    float count = 0;
    
    for (float py = -shiftNum / 2; py <= shiftNum / 2;py++)
    {
        for (float px = -shiftNum / 2; px <= shiftNum / 2; px++)
        {
            col += tex.Sample(smp, input.uv + float2(px, py) * shiftWidth);
            count++;
        }
    }
    
    col.rgb /= count;
    col.a = 1;
    
    return col;
}