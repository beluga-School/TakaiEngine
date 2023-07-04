cbuffer cbuff0 : register(b0)
{
    float4 colorAndThickness;
}

cbuffer cbuff1 : register(b1)
{
    matrix viewproj; //ビュープロ行列
    matrix world; //ワールド行列
    float3 cameraPos; //カメラ座標(ワールド座標)
    float3 cameraDir; //カメラの視線ベクトル
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldPos : POSITION; //ワールド座標
    float3 normal : NORMAL; //法線
    float2 uv : TEXCOORD;
    float3 viewDir : TEXCOORD1;
};