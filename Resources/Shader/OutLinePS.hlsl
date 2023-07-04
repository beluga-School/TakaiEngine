#include "OutLine.hlsli"

float4 main() : SV_TARGET
{
    return float4(colorAndThickness.rgb,1.0f);
}