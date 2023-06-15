#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"
#include "PointLight.h"
#include <stdlib.h>
#include <memory>

//���C�g�̐�
static const int32_t sDirLightNum = 3;

//�_�����̐�
static const int32_t sPointLightNum = 3;

struct LightGroupData
{
	Vector3 mAmbienColor;
	float mPad1;

	DirectionalLight::ConstBufferData mDirLights[sDirLightNum];

	PointLight::ConstBufferData mPointLights[sPointLightNum];
};

class LightGroup
{
public://�ϐ�
	

	//�S�ẴI�u�W�F�N�g�ŋ��ʂ̃��C�g�f�[�^
	static std::unique_ptr<LightGroup> mLightGroup;
	/*static void SetLight(std::unique_ptr<LightGroup> light) {
		lightGroup = light;
	};*/
	   
	//�萔�o�b�t�@
	ConstBuffer<LightGroupData> mConstBuff;

public://�֐�
	//�C���X�^���X����
	static void Create();

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��R�}���h
	void Draw(const UINT& index);

	//�萔�o�b�t�@��]��
	void TransferBuffer();

	//���s����
	//���C�g�̐F��ݒ�
	void SetAmbientColor(const Vector3& color);

	//���s�����̗L���t���O���Z�b�g
	void SetDirLightActive(const int32_t& index, const bool& active);

	//���s�����̃��C�g�������Z�b�g
	void SetDirLightDir(const int32_t& index, const Vector3& lightdir);
	
	//���s�����̃��C�g�������Z�b�g
	void SetDirLightColor(const int32_t& index, const Vector3& lightcolor);

	//�_����
	
	void SetPointLightActive(const int32_t& index, const bool& active);
	void SetPointLightPos(const int32_t& index, const Vector3& pos);
	void SetPointLightColor(const int32_t& index, const Vector3& color);
	void SetPointLightAtten(const int32_t& index, const Vector3& atten);

	//�W���̃��C�g�ݒ�
	void DefaultLightSet();

	DirectionalLight mDirLights[sDirLightNum];
	PointLight mPointLights[sDirLightNum];

private:
	//�����̐F
	Vector3 mAmbienColor = { 1,1,1 };

	bool mDirty = false;
};

