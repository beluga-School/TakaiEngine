#pragma once
#include <string>
#include "Vector3.h"
#include "Texture.h"

struct Material
{
	std::string name;			//�}�e���A���̖��O
	Vector3 ambient;			//�A���r�G���g�e���x
	Vector3 diffuse;			//�f�B�t���[�Y
	Vector3 specular;			//�X�y�L�����[�e���x
	float alpha;				//�A���t�@
	std::string textureFilename;//�e�N�X�`���t�@�C����
	Texture tex;

	Material() {
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
	}
};