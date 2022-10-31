cbuffer cbuff0 : register(b0)
{
    matrix mat;
};

struct VSOutput
{
    float4 pos : POSITION;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};