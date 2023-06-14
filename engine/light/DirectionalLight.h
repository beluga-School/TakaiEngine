#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ConstBuffer.h"

class DirectionalLight
{
public://�ϐ�
	Vector3 direction = { 1,0,0 };
	Vector3 color = { 1,1,1 };

	struct ConstBufferData
	{
		Vector3 direction;
		bool active;
		Vector3 color;
		float pad;
	};

public://�֐�
	static DirectionalLight* Create() {
		//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
		DirectionalLight* instance = new DirectionalLight();
		//������
		instance->Initialize();
		//���������C���X�^���X��Ԃ�
		return instance;
	};

	void Initialize();
	void Update();
	void Draw(const UINT& rootParameterIndex);

	void TransferBuffer();

	void SetLightDirection(const Vector3& lightdir);
	void SetLightColor(const Vector3& color);

	bool active = false;

private://�ϐ�
	//�萔�o�b�t�@
	ConstBuffer<ConstBufferData> constBuff;

	bool dirty = false;
};