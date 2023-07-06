cbuffer cbuff0 : register(b0)
{
    matrix viewproj; //�r���[�v���s��
    matrix world; //���[���h�s��
    float4 color; //�J���[
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldPos : POSITION; //���[���h���W
};