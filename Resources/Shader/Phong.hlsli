cbuffer cbuff0 : register(b0)
{
    float3 m_ambient : packoffset(c0); //�A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); //�f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); //�X�y�L�����[�W��
    float m_alpha : packoffset(c2.w); //�A���t�@
}

cbuffer cbuff1 : register(b1)
{
    //matrix mat; //3D�ϊ��s��
    matrix viewproj;    //�r���[�v���s��
    matrix world;       //���[���h�s��
    float3 cameraPos;   //�J�������W(���[���h���W)
};

cbuffer cbuff2 : register(b2)
{
    float4 color; //�J���[
};

struct DirLight
{
    float3 lightv;
    float3 lightcolor;
    uint active;
};

cbuffer cbuff3 : register(b3)
{
    //float3 lightv : packoffset(c0); //���C�g�̕����x�N�g��
    //float3 lightcolor : packoffset(c1); //���C�g�̐F(RGB)
    float3 ambientColor : packoffset(c0);
    DirLight dirLights[3] : packoffset(c1);
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    //float4 color : COLOR;
    float4 worldPos : POSITION; //���[���h���W
    float3 normal : NORMAL;     //�@��
    float2 uv : TEXCOORD;
};