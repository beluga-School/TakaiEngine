#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"
#include "PointLight.h"
#include <stdlib.h>
#include <memory>

//���C�g�̐�
static const int32_t sDIRLIGHT_NUM = 3;

//�_�����̐�
static const int32_t sPOINTLIGHT_NUM = 3;

struct LightGroupData
{
	Vector3 pad3;
	float pad;

	DirectionalLight::ConstBufferData mDirLights[sDIRLIGHT_NUM];

	PointLight::ConstBufferData mPointLights[sPOINTLIGHT_NUM];
};

class LightGroup
{
public://�ϐ�
	

	//�S�ẴI�u�W�F�N�g�ŋ��ʂ̃��C�g�f�[�^
	static std::unique_ptr<LightGroup> sLightGroup;
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

	DirectionalLight mDirLights[sDIRLIGHT_NUM];
	PointLight mPointLights[sDIRLIGHT_NUM];

private:

	bool mDirty = false;
};

