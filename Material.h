#pragma once
#include <string>
#include <memory>
#include "Vector3.h"
#include "Texture.h"

struct Material
{
	std::string name;			//�}�e���A���̖��O
	Vector3 ambient;			//�A���r�G���g�e���x
	Vector3 diffuse;			//�f�B�t���[�Y
	Vector3 specular;			//�X�y�L�����[�e���x
	float alpha;				//�A���t�@
	Vector3 color;
	std::string textureFilename;//�e�N�X�`���t�@�C����
	std::unique_ptr<Texture> tex = std::make_unique<Texture>();

	Material() {
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
		color = { 1.0f,1.0f,1.0f };
	}
};