#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ConstBuffer.h"

struct LightConstBufferData
{
	Vector3 direction;
	Vector3 color;
};

class Light
{
public:
	static Light* Create() {
		//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
		Light* instance = new Light();
		//������
		instance->Initialize();
		//���������C���X�^���X��Ԃ�
		return instance;
	};

	void Initialize();
	void Update();
	void Draw(UINT rootParameterIndex);

	void TransferBuffer();

	void SetLightDirection(Vector3 lightdir);

private:
	//�萔�o�b�t�@
	ConstBuffer<LightConstBufferData> constBuff;

	Vector3 direction = { 1,0,0};
	Vector3 color = { 1,1,1 };

	bool dirty = false;
};