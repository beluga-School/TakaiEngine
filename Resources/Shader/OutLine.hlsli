cbuffer cbuff0 : register(b0)
{
    float4 color;
    float thickness;
}

cbuffer cbuff1 : register(b1)
{
    matrix view; //ビュー行列単体
    matrix viewproj; //ビュープロジェクション行列
    matrix world; //ワールド行列
    float3 cameraPos; //カメラ座標(ワールド座標)
    float2 tiling; //タイリング
    float2 offset; //オフセット
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldPos : POSITION; //ワールド座標
    float3 normal : NORMAL; //法線
    float2 uv : TEXCOORD;
    float3 viewDir : TEXCOORD1;
};