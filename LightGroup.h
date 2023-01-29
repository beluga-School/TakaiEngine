#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"

//���C�g�̐�
static const int DirLightNum = 3;

struct LightGroupData
{
	Vector3 ambienColor;
	float pad1;

	DirectionalLight dirLights[DirLightNum];
};

class LightGroup
{
public://�ϐ�
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

	//���C�g�̐F��ݒ�
	void SetAmbientColor(Vector3& color);

	//���s�����̗L���t���O���Z�b�g
	void SetDirLightActive(int index,bool active);

	//���s�����̃��C�g�������Z�b�g
	void SetDirLightDir(int index, Vector3 lightdir);
	
	//���s�����̃��C�g�������Z�b�g
	void SetDirLightColor(int index, Vector3 lightcolor);

	//�W���̃��C�g�ݒ�
	void DefaultLightSet();

private:
	//�����̐F
	Vector3 ambienColor = { 1,1,1 };

	DirectionalLight dirLights[DirLightNum];

	bool dirty = false;
};

