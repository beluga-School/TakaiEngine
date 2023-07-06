cbuffer cbuff0 : register(b0)
{
    matrix viewproj; //ビュープロ行列
    matrix world; //ワールド行列
    float4 color; //カラー
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldPos : POSITION; //ワールド座標
};