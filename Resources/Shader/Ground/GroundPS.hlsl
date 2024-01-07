#include "Ground.hlsli"

Texture2D<float4> tex : register(t0);
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

    int ditherUV_x = (int) fmod(input.svpos.x, 4.0f); //�p�^�[���̑傫���Ŋ������Ƃ��̗]������߂�
    int ditherUV_y = (int) fmod(input.svpos.y, 4.0f); //����̃p�^�[���T�C�Y��4x4�Ȃ̂�4�ŏ��Z
    float dither = pattern[ditherUV_x + ditherUV_y * 4]; //���߂��]�肩��p�^�[���l���擾
    
    float camlength = magnitude(cameraPos - input.worldPos.xyz);
    
    float clamp_dither = saturate(dither / 4);
    float clamp_length = saturate(camlength / 50);
    
    clip(clamp_length - clamp_dither); //臒l��0�ȉ��Ȃ�`�悵�Ȃ�
    
    clip(m_alpha);
  
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
    
    for (int j = 0; j < POINTLIGHT_NUM; j++)
    {
        if (pointLights[j].active)
        {
            //�x�N�g���̒���
            float distance = length(pointLights[j].lightpos - input.worldPos.xyz);
            float factor = pow(saturate(-distance / pointLights[j].radius + 1.0f), pointLights[j].decay);
            
            float3 pointColor = pointLights[j].lightcolor.rgb * pointLights[j].intensity * factor;
            
            float3 diffuse = pointColor * m_diffuse;
        
            float3 specular = pointColor * float3(1.0f, 1.0f, 1.0f);
           
            shadecolor.rgb += diffuse + specular;
        }
    }
    
    for (i = 0; i < CIRCLESHADOW_NUM; i++)
    {
        if (circleShadows[i].active)
        {
            float3 casterv = circleShadows[i].casterPos - input.worldPos.xyz;
            
            float d = dot(casterv, circleShadows[i].direction);
            
            float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
            
            atten *= step(0, d);
            
            float3 lightpos = circleShadows[i].casterPos + circleShadows[i].direction * circleShadows[i].distance;
            
            float3 lightv = normalize(lightpos - input.worldPos.xyz);
            
            float cos = dot(lightv, circleShadows[i].direction);
            
            float angleatten = smoothstep(circleShadows[i].cosAngle.y, circleShadows[i].cosAngle.x, cos);
            
            atten *= angleatten;
   
            shadecolor.rgb -= atten * float3(0.1f, 0.1f, 0.1f);
        }
    }
    
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
    float3 outputColor = (shadecolor.rgb * color.rgb) + float3(fogColor, fogColor, fogColor);
    
    //�o��
    return float4(outputColor, color.a * texcolor.a);
    //return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
}