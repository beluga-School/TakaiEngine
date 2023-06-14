#pragma once
#include <string>
#include <memory>
#include "Vector3.h"
#include "Texture.h"

struct Material
{
	std::string mName;			//マテリアルの名前
	Vector3 mAmbient;			//アンビエント影響度
	Vector3 mDiffuse;			//ディフューズ
	Vector3 mSpecular;			//スペキュラー影響度
	float mAlpha;				//アルファ
	Vector3 mColor;
	std::string mTextureFilename;//テクスチャファイル名
	std::unique_ptr<Texture> mTextire = std::make_unique<Texture>();

	Material() {
		mAmbient = { 0.3f,0.3f,0.3f };
		mDiffuse = { 0.0f,0.0f,0.0f };
		mSpecular = { 0.0f,0.0f,0.0f };
		mAlpha = 1.0f;
		mColor = { 1.0f,1.0f,1.0f };
	}
};