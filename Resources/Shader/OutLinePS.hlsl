#include "OutLine.hlsli"

static const int pattern[16] =
{
    0, 8, 2, 10,
    12, 4, 14, 6,
    3, 11, 1, 9,
    15, 7, 13, 5
};

float3 magnitude(float3 v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

float4 main(VSOutput input) : SV_TARGET
{
    int ditherUV_x = (int) fmod(input.svpos.x, 4.0f); //�p�^�[���̑傫���Ŋ������Ƃ��̗]������߂�
    int ditherUV_y = (int) fmod(input.svpos.y, 4.0f); //����̃p�^�[���T�C�Y��4x4�Ȃ̂�4�ŏ��Z
    float dither = pattern[ditherUV_x + ditherUV_y * 4]; //���߂��]�肩��p�^�[���l���擾
    
    float camlength = magnitude(cameraPos - input.worldPos.xyz);
    
    float clamp_dither = saturate(dither / 16);
    float clamp_length = saturate(camlength / 20);
    
    clip(clamp_length - clamp_dither); //臒l��0�ȉ��Ȃ�`�悵�Ȃ�
    
    return color;
}