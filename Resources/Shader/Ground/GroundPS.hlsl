#include "Ground.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float> depthTex : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);

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
    float4 texcolor = tex.Sample(smp, input.uv * tiling + offset);
    float maskTexcolor = depthTex.Sample(smp, input.uv * tiling + offset);
    
    int ditherUV_x = (int) fmod(input.svpos.x, 4.0f); //�p�^�[���̑傫���Ŋ������Ƃ��̗]������߂�
    int ditherUV_y = (int) fmod(input.svpos.y, 4.0f); //����̃p�^�[���T�C�Y��4x4�Ȃ̂�4�ŏ��Z
    float dither = pattern[ditherUV_x + ditherUV_y * 4]; //���߂��]�肩��p�^�[���l���擾
    
    //bool checkPZ = input.camWorldPos.z > input.camPlayerPos.z;
    //if (input.camWorldPos.z < input.camPlayerPos.z)
    //{
    //    discard;   
    //}
    
    float camlength = magnitude(playerpos - input.worldPos.xyz);
    
    float clamp_dither = saturate(dither / 16);
    float clamp_length = saturate(camlength / 200);
    
    clip(clamp_length - clamp_dither); //臒l��0�ȉ��Ȃ�`�悵�Ȃ�
    //clip(1 - checkPZ);
    
    float4 shadecolor = { 0, 0, 0, 1 };
    
    float threshold = 0.01f;
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            float3 ambient = texcolor.rgb * m_ambient * dirLights[i].lightcolor;
            
            float intensity = step(threshold, saturate(dot(normalize(input.normal), dirLights[i].lightv)));
            
            float3 difColor = texcolor.rgb;
            
            float3 diffuse = difColor * intensity * dirLights[i].lightcolor;
 
            shadecolor.rgb += (ambient + diffuse);
            shadecolor.a += 1;
        }
    }
    
    return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
    //return float4(maskTexcolor, 0, 0, 1);
}