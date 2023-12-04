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
    float2 tiling;  //タイリング
    float2 offset;  //オフセット
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
    float4 lightcolor;
    float3 lightpos;
    float intensity;
    float radius;
    float decay;
    uint active;
};

static const int SPOTLIGHT_NUM = 3;

struct SpotLight
{
    float4 lightcolor;
    float3 lightpos;
    float intensity;
    float3 direction;
    float distance;
    float decay;
    float cosAngle;
    uint active;
};

cbuffer cbuff3 : register(b3)
{
    float3 pad;
    DirLight dirLights[DIRLIGHT_NUM];
    PointLight pointLights[POINTLIGHT_NUM];
    SpotLight spotLights[SPOTLIGHT_NUM];
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldPos : POSITION; //ワールド座標
    float3 normal : NORMAL; //法線
    float2 uv : TEXCOORD;
    float3 viewDir : TEXCOORD1;
};