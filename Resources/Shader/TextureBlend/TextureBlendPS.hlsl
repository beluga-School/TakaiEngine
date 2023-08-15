#include "TextureBlend.hlsli"

//�e�N�X�`����3�����Ă��Ȃ��Ƃ����Ȃ�
Texture2D<float4> maintex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> subtex : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> masktex : register(t2); //2�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 main = maintex.Sample(smp, input.uv * tiling);
    float4 sub = subtex.Sample(smp, input.uv * tiling);
    float4 mask = masktex.Sample(smp, input.uv * tiling);
    
    float4 shadecolor = { 0, 0, 0, 1 };
    float4 texcolor = { 0, 0, 0, 1 };
    
    //�}�X�N�̐Ԃ�������subTexture�ɁA����������mainTexture��
    texcolor = mask.r * sub + (1 - mask.r) * main;
    
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

    return shadecolor * color;
}