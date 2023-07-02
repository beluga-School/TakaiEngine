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
    float3 solDir = normalize(solPos - input.worldPos.xyz);
    float3 ambient = m_ambient;
    //float3 ambient = float3(0.0f,0.0f,1.0f);
    
    //���_�x�N�g��
    float3 eyeDir = normalize(input.worldPos.xyz - cameraPos);
    
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
            float3 diffuse = smoothstep(t, t + 0.05f, dotlightnormal) * float3(0.1f, 0.1f, 0.1f);
            
            //���z���ŃX�y�L�����[�������Ȃ��悤��(�Ȃ񂩊Ԉ���ĂȂ��H)
            float3 specular = smoothstep(0.15f, 0.2f, pow(saturate(dot(reflect, eyeDir)), shininess)) * m_specular;
            
           
            //shadecolor.rgb += diffuse * dirLights[i].lightcolor;
           
            
            
            //���Z
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
          
            
            ////�������C�g
            //float rimPower = 2.0f;
                    
            //float3 eye_vector = normalize(-abs(cameraDir));
            //float3 light_vector = normalize(dirLights[i].lightv);
            //float3 normal = normalize(input.normal);
            
            //float rim = 1.0f - dot(normal, eye_vector);
            
            //rim = pow(abs(rim), rimPower);
            
            //float light_rim = max(dot(-light_vector, eye_vector), 0.0f);
            
            //float3 rimcolor = rim * light_rim;
            ////�������C�g�͂킯�ĉ��Z���Ă݂�
            //shadecolor.rgb += rimcolor;
            
            //�|�C���g���C�g�͎g���ĂȂ��̂ł����������
        }
    }
    
    

    return shadecolor * texcolor * color;
}