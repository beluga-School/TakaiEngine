#pragma once
#include <string>
#include "Vector3.h"
#include "Texture.h"

struct Material
{
	std::string name;			//マテリアルの名前
	Vector3 ambient;			//アンビエント影響度
	Vector3 diffuse;			//ディフューズ
	Vector3 specular;			//スペキュラー影響度
	float alpha;				//アルファ
	std::string textureFilename;//テクスチャファイル名
	Texture tex;

	Material() {
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
	}
};