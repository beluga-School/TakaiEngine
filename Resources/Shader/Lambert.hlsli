cbuffer cbuff0 : register(b0)
{
    float3 m_ambient : packoffset(c0); //アンビエント係数
    float3 m_diffuse : packoffset(c1); //ディフューズ係数
    float3 m_specular : packoffset(c2); //スペキュラー係数
    float m_alpha : packoffset(c2.w); //アルファ
}

cbuffer cbuff1 : register(b1)
{
    //matrix mat; //3D変換行列
    matrix viewproj; //ビュープロ行列
    matrix world; //ワールド行列
    float3 cameraPos; //カメラ座標(ワールド座標)
};

cbuffer cbuff2 : register(b2)
{
    float4 color; //カラー
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};