#include "Toon.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
    //����x
    const float shininess = 4.0f;
    
    //�O�̏����@���������l�������Ɨ��������炱������������
    //float3 eyedir = normalize(cameraPos - input.worldPos.xyz);
    
    //cameraPos�Q�Ƃ��ƁA��肽�����ƂƊ��ݍ����ĂȂ������̂ŏ���ɑ��z�̈ʒu������
    float3 solPos = { 0, 100, 0 };
    float3 eyedir = normalize(solPos - input.worldPos.xyz);
    float3 ambient = m_ambient;
    //float3 ambient = float3(0.0f,0.0f,1.0f);
    
    float t = 0.01f;
    
    //�V�F�[�f�B���O�ɂ��F
    float3 ambientColor = float3(0.8f, 0.8f, 0.8f);
    
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
            
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
            
            //�g�D�[���͂�����������Ȃ��Ƃ����Ȃ�
            float3 diffuse = step(t,dotlightnormal) * float3(0.1f,0.1f,0.1f);
            
            float3 specular = step(0.2f,pow(saturate(dot(reflect, eyedir)), shininess)) * m_specular;
            
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
            //shadecolor.rgb += diffuse * dirLights[i].lightcolor;
        }
    }
    //�|�C���g���C�g�͎g���ĂȂ��̂ł����������

    return shadecolor * texcolor * color;
}