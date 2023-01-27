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
    
cbuffer cbuff3 : register(b3)
{
    float3 lightv : packoffset(c0); //���C�g�̕����x�N�g��
    float3 lightcolor : packoffset(c1); //���C�g�̐F(RGB)
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};