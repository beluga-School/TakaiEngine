#include "BackGround.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv * tiling + offset);
    clip(texcolor.a);
    
    //�J�����ƒ��_�̋������v�Z
    float3 camLength = (cameraPos - input.worldPos.xyz);
    camLength = abs(camLength);
    
    //�t�H�O�����S�ɂ����鋗��
    const float FOG_MAX = 1000.0f;
    //�t�H�O��������n�߂鋗��
    const float FOG_START = 50.f;
    
    //�t�H�O�F�̋������Z�o
    float fogWeight = (camLength.x + camLength.z) / 2 - FOG_START;
    fogWeight = max(fogWeight, 0);
    
    //�t�H�O�F�̋����������鋗���Ŋ����ď����߂̒l�ɕϊ�(������ӓK���B������Ƃ��Ȃ�0~1�̒l�ɂ���������܂�悤�ɂ�����)
    float fogColor = fogWeight / FOG_MAX;

    //�ŏI�I�ȐF���Z�o(�ۉe�Ƃ��̐F���o�����l�ɖ������悹��`�Ȃ̂ŁA���������肪�N�������B���킢�B)
    float3 outputColor = (texcolor.rgb * color.rgb) + float3(fogColor, fogColor, fogColor);
    
    //�o��
    return float4(outputColor, color.a * texcolor.a);
}