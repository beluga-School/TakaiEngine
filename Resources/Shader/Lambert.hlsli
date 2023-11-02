#include "BasicConstBuffer.hlsli"

cbuffer cbuff2 : register(b2)
{
    float4 color; //ÉJÉâÅ[
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};