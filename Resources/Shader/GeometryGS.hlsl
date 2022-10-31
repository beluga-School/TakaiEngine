#include "GeometryHeader.hlsli"

static const uint vnum = 4;

static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0),   
    float4(-0.5f, +0.5f, 0, 0),   
    float4(+0.5f, -0.5f, 0, 0),   
    float4(+0.5f, +0.5f, 0, 0)
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
    GSOutput element;
	
	for (uint i = 0; i < vnum; i++)
	{
        element.svpos = input[0].pos + offset_array[i];
        element.svpos = mul(mat, element.svpos);
        element.uv = float2(0.5f, 0.5f);
        output.Append(element);
    }
}