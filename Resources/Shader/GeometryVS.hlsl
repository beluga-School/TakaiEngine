#include "GeometryHeader.hlsli"

VSOutput main( float4 pos : POSITION )
{
    VSOutput output;
    output.pos = pos;
    return output;
}