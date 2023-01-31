#pragma once
#include "Vector3.h"

class PointLight
{
public://�萔�o�b�t�@�ɑ���f�[�^
	struct ConstBufferData
	{
		Vector3 lightPos;
		float pad1;
		Vector3 lightColor;
		float pad2;
		Vector3 lighttAtten;
		bool active;
	};
public://�����o�ϐ�
	//���C�g���W
	Vector3 lightPos = {0,0,0};
	//���C�g�̐F
	Vector3 lightColor = { 1,1,1 };
	//���C�g���������W��
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	
	bool active = false;
};
