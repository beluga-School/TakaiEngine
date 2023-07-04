cbuffer cbuff0 : register(b0)
{
    float4 colorAndThickness;
}

cbuffer cbuff1 : register(b1)
{
    matrix viewproj; //�r���[�v���s��
    matrix world; //���[���h�s��
    float3 cameraPos; //�J�������W(���[���h���W)
    float3 cameraDir; //�J�����̎����x�N�g��
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldPos : POSITION; //���[���h���W
    float3 normal : NORMAL; //�@��
    float2 uv : TEXCOORD;
    float3 viewDir : TEXCOORD1;
};