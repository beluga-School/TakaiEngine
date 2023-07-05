#include "Phong.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
    float4 shadecolor = { 0, 0, 0, 1 };
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            //�A���r�G���g = �I�u�W�F�N�g�̌��̐F * �Â����闦 * ���̐F
            float3 ambient = texcolor.rgb * m_ambient * dirLights[i].lightcolor;
            
            //�f�B�t���[�Y
            //���Ƃ̓��ϒl
            float intensity = saturate(dot(normalize(input.normal), dirLights[i].lightv));
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
            
            float3 specular = pow(saturate(dot(reflectDir, eyeDir)), 20) * dirLights[i].lightcolor;
            
            shadecolor.rgb += (ambient + diffuse + specular);
            shadecolor.a += 1;
        }
    }

    return shadecolor * texcolor * color;
    
    //for (i = 0; i < POINTLIGHT_NUM; i++)
    //{
    //    if (pointLights[i].active)
    //    {
    //        //���C�g�ւ̃x�N�g��
    //        float3 lightv = pointLights[i].lightpos - input.worldPos.xyz;
    //        lightv = normalize(lightv);
    //        //�x�N�g���̒���
    //        float d = length(lightv);
    //        //���������W��
    //        float atten = 1.0f / (
    //        pointLights[i].lightatten.x +
    //        pointLights[i].lightatten.y * d +
    //        pointLights[i].lightatten.z * d * d);
            
    //        float3 dotlightnormal = dot(lightv, input.normal);
            
    //        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
  
    //        float3 diffuse = dotlightnormal * m_diffuse;
    
    //        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
    //        shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
            
    //    }
    //}
}