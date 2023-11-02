cbuffer cbuff0 : register(b0)
{
    float3 m_ambient : packoffset(c0); //アンビエント係数
    float3 m_diffuse : packoffset(c1); //ディフューズ係数
    float3 m_specular : packoffset(c2); //スペキュラー係数
    float m_alpha : packoffset(c2.w); //アルファ
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