cbuffer cbuff0 : register(b0)
{
    float3 m_ambient : packoffset(c0); //�A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); //�f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); //�X�y�L�����[�W��
    float m_alpha : packoffset(c2.w); //�A���t�@
}

cbuffer cbuff1 : register(b1)
{
    matrix view; //�r���[�s��P��
    matrix viewproj; //�r���[�v���W�F�N�V�����s��
    matrix world; //���[���h�s��
    float3 cameraPos; //�J�������W(���[���h���W)
    float2 tiling; //�^�C�����O
    float2 offset; //�I�t�Z�b�g
};