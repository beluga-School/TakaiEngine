cbuffer cbuff0 : register(b0)
{
    float3 m_ambient : packoffset(c0); //�A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); //�f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); //�X�y�L�����[�W��
    float m_alpha : packoffset(c2.w); //�A���t�@
}

cbuffer cbuff1 : register(b1)
{
    matrix viewproj; //�r���[�v���s��
    matrix world; //���[���h�s��
    float3 cameraPos; //�J�������W(���[���h���W)
    float2 tiling; //�^�C�����O
};

cbuffer cbuff2 : register(b2)
{
    float4 color; //�J���[
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

//�v���C���[�ƈʒu���������A�s�N�Z����`��L�����Z������
cbuffer cbuff4 : register(b4)
{
    float4 playerPosAndRadius;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldPos : POSITION; //���[���h���W
    float3 normal : NORMAL; //�@��
    float2 uv : TEXCOORD;
    float3 viewDir : TEXCOORD1;
};