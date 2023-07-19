#include "Disolve.hlsli"

Texture2D<float4> mainTex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> maskTex : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 main = mainTex.Sample(smp, input.uv);
    float4 mask = maskTex.Sample(smp, input.uv);
    
    float4 shadecolor = { 0, 0, 0, 1 };

    //���̃s�N�Z���̃}�X�N�̐Ԑ������Q�Ƃ��āA�`��j�����s��
    clip(mask.r - disolve);
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            //�A���r�G���g = �I�u�W�F�N�g�̌��̐F * �Â����闦 * ���̐F
            float3 ambient = main.rgb * m_ambient * dirLights[i].lightcolor;
            
            //�f�B�t���[�Y
            //���Ƃ̓��ϒl
            float intensity = saturate(dot(normalize(input.normal), dirLights[i].lightv));
            //�I�u�W�F�N�g�̐F
            float3 difColor = main.rgb;
            
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
    
    //�ǂ����������H
    return shadecolor * color;
}