#include "BasicConstBuffer.hlsli"

cbuffer cbuff2 : register(b2)
{
    float4 color; //カラー
};

static const int DIRLIGHT_NUM = 3;

struct DirLight
{
    float3 lightv;
    uint active;
    float3 lightcolor;
};

static const int POINTLIGHT_NUM = 3;

struct PointLight
{
    float3 lightpos;
    float3 lightcolor;
    float3 lightatten;
    uint active;
};

cbuffer cbuff3 : register(b3)
{
    float3 pad;
    DirLight dirLights[DIRLIGHT_NUM];
    PointLight pointLights[POINTLIGHT_NUM];
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldPos : POSITION; //ワールド座標
    float3 normal : NORMAL;     //法線
    float2 uv : TEXCOORD;
};