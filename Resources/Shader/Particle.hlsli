cbuffer cbuff0 : register(b0)
{
    matrix vpMat;
    matrix billMat;
};

struct VSOutput
{
    float4 pos : POSITION;
    float scale : SCALE;
    float4 color : COLOR;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};