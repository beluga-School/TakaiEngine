#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"
#include "PointLight.h"

//���C�g�̐�
static const int DirLightNum = 3;

//�_�����̐�
static const int PointLightNum = 3;

struct LightGroupData
{
	Vector3 ambienColor;
	float pad1;

	DirectionalLight::ConstBufferData dirLights[DirLightNum];

	PointLight::ConstBufferData pointLights[PointLightNum];
};

class LightGroup
{
public://�ϐ�
	//�S�ẴI�u�W�F�N�g�ŋ��ʂ̃��C�g�f�[�^
	static LightGroup* lightGroup;
	static void SetLight(LightGroup* light) {
		lightGroup = light;
	};
	   
	//�萔�o�b�t�@
	ConstBuffer<LightGroupData> constBuff;

public://�֐�
	//�C���X�^���X����
	static LightGroup* Create();

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��R�}���h
	void Draw(UINT index);

	//�萔�o�b�t�@��]��
	void TransferBuffer();

	//���s����
	//���C�g�̐F��ݒ�
	void SetAmbientColor(const Vector3& color);

	//���s�����̗L���t���O���Z�b�g
	void SetDirLightActive(int index, const bool& active);

	//���s�����̃��C�g�������Z�b�g
	void SetDirLightDir(int index, const Vector3& lightdir);
	
	//���s�����̃��C�g�������Z�b�g
	void SetDirLightColor(int index, const Vector3& lightcolor);

	//�_����
	
	void SetPointLightActive(int index, const bool& active);
	void SetPointLightPos(int index, const Vector3& pos);
	void SetPointLightColor(int index, const Vector3& color);
	void SetPointLightAtten(int index, const Vector3& atten);

	//�W���̃��C�g�ݒ�
	void DefaultLightSet();

	DirectionalLight dirLights[DirLightNum];
	PointLight pointLights[DirLightNum];

private:
	//�����̐F
	Vector3 ambienColor = { 1,1,1 };

	bool dirty = false;
};

