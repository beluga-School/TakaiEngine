#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��	�@���[�g�p�����[�^
SamplerState smp : register(s0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp,input.uv));
	float3 light = normalize(float3(1,-1,1));		//�E�����@�����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;	//�����ւ̃x�N�g���ƃx�N�g���̓���
	return float4(texcolor.rgb * brightness, texcolor.a) * color;//�P�x��RGB�ɑ�����ďo��

	//return float4(input.normal,1);	//RGB�����ꂼ��@����XYZ�AA��1�ŏo��
	//return float4(input.uv,0,1);
	//return float4(1,1,1,1);
}