#include "CharactorDT.hlsli"

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
    
    float camlength = magnitude(cameraPos - input.worldPos.xyz);
    
    float clamp_dither = saturate(dither / 16);
    float clamp_length = saturate(camlength / 50);
    
    clip(clamp_length - clamp_dither); //臒l��0�ȉ��Ȃ�`�悵�Ȃ�

    float4 shadecolor = { 0, 0, 0, 1 };
    
    float threshold = 0.01f;
       
    //�X�y�L�����[�������l
    float speThreshold = 0.2f;
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            //�A���r�G���g = �I�u�W�F�N�g�̌��̐F * �Â����闦 * ���̐F
            float3 ambient = texcolor.rgb * m_ambient * dirLights[i].lightcolor;
            
            //�f�B�t���[�Y
            //���Ƃ̓��ϒl
            float intensity = step(threshold, saturate(dot(normalize(input.normal), dirLights[i].lightv)));
            //�I�u�W�F�N�g�̐F
            float3 difColor = texcolor.rgb;
            
            //�f�B�t���[�Y = �I�u�W�F�N�g�̌��̐F * ���Ƃ̓��ϒl * ���̐F
            float3 diffuse = difColor * intensity * dirLights[i].lightcolor;
            
            //�X�y�L�����[
            //�����x�N�g��
            float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
            //���ˌ�
            float3 lightDir = normalize(dirLights[i].lightv);
            
            float3 normalizeVec = normalize(input.normal);
            //���ˌ�
            float3 reflectDir = -lightDir + 2 * normalizeVec * dot(normalizeVec, lightDir);
            
            float3 specular = step(speThreshold, pow(saturate(dot(reflectDir, eyeDir)), 20) * dirLights[i].lightcolor);
            
            shadecolor.rgb += (ambient + diffuse + specular);
            shadecolor.a += 1;
        }
    }
    
    return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
    //return float4(maskTexcolor, 0, 0, 1);
}