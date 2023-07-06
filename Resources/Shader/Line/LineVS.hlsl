#include "Line.hlsli"

VSOutput main(float4 pos : POSITION)
{
    //法線にワールド行列によるスケーリング・回転を適用
    float4 wpos = mul(world, pos);
    
    VSOutput output;
    output.svpos = mul(viewproj, wpos);
    output.worldPos = wpos;
    
    return output;
}