#pragma once
#include <string>
#include <memory>
#include "Vector3.h"
#include "Texture.h"

struct Material
{
	std::string mName;			//�}�e���A���̖��O
	Vector3 mAmbient;			//�A���r�G���g�e���x
	Vector3 mDiffuse;			//�f�B�t���[�Y
	Vector3 mSpecular;			//�X�y�L�����[�e���x
	float mAlpha;				//�A���t�@
	Vector3 mColor;
	std::string mTextureFilename;//�e�N�X�`���t�@�C����
	std::unique_ptr<Texture> mTextire = std::make_unique<Texture>();

	Material() {
		mAmbient = { 0.3f,0.3f,0.3f };
		mDiffuse = { 0.0f,0.0f,0.0f };
		mSpecular = { 0.0f,0.0f,0.0f };
		mAlpha = 1.0f;
		mColor = { 1.0f,1.0f,1.0f };
	}
};