cbuffer cbuff0 : register(b0)
{
    float3 m_ambient : packoffset(c0); //アンビエント係数
    float3 m_diffuse : packoffset(c1); //ディフューズ係数
    float3 m_specular : packoffset(c2); //スペキュラー係数
    float m_alpha : packoffset(c2.w); //アルファ
}

cbuffer cbuff1 : register(b1)
{
    matrix viewproj; //ビュープロ行列
    matrix world; //ワールド行列
    float3 cameraPos; //カメラ座標(ワールド座標)
    float2 tiling; //タイリング
};

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

//プレイヤーと位置が被ったら、ピクセルを描画キャンセルする
cbuffer cbuff4 : register(b4)
{
    float4 playerPosAndRadius;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldPos : POSITION; //ワールド座標
    float3 normal : NORMAL; //法線
    float2 uv : TEXCOORD;
    float3 viewDir : TEXCOORD1;
};