#include "Phong.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
    //����x
    const float shininess = 4.0f;
    
    float3 eyedir = normalize(cameraPos - input.worldPos.xyz);
    float3 ambient = m_ambient;
    
    //�V�F�[�f�B���O�ɂ��F
    float4 shadecolor = float4(ambientColor * ambient,m_alpha);
    
    for (int i = 0; i < 3; i++)
    {
        if(dirLights[i].active)
        {
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
            
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
  
            float3 diffuse = dotlightnormal * m_diffuse;
    
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
        }
    }
    
    return shadecolor * texcolor * color;
}