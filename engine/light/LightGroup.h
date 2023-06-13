#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"
#include "PointLight.h"
#include <stdlib.h>
#include <memory>

//���C�g�̐�
static const int32_t DirLightNum = 3;

//�_�����̐�
static const int32_t PointLightNum = 3;

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
	static std::unique_ptr<LightGroup> lightGroup;
	/*static void SetLight(std::unique_ptr<LightGroup> light) {
		lightGroup = light;
	};*/
	   
	//�萔�o�b�t�@
	ConstBuffer<LightGroupData> constBuff;

public://�֐�
	//�C���X�^���X����
	static void Create();

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
	void SetDirLightActive(int32_t index, const bool& active);

	//���s�����̃��C�g�������Z�b�g
	void SetDirLightDir(int32_t index, const Vector3& lightdir);
	
	//���s�����̃��C�g�������Z�b�g
	void SetDirLightColor(int32_t index, const Vector3& lightcolor);

	//�_����
	
	void SetPointLightActive(int32_t index, const bool& active);
	void SetPointLightPos(int32_t index, const Vector3& pos);
	void SetPointLightColor(int32_t index, const Vector3& color);
	void SetPointLightAtten(int32_t index, const Vector3& atten);

	//�W���̃��C�g�ݒ�
	void DefaultLightSet();

	DirectionalLight dirLights[DirLightNum];
	PointLight pointLights[DirLightNum];

private:
	//�����̐F
	Vector3 ambienColor = { 1,1,1 };

	bool dirty = false;
};

