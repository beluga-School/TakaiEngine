#pragma once
#include <string>
#include <memory>
#include "Vector3.h"
#include "Texture.h"

struct Material
{
	std::string name;			//マテリアルの名前
	Vector3 ambient;			//アンビエント影響度
	Vector3 diffuse;			//ディフューズ
	Vector3 specular;			//スペキュラー影響度
	float alpha;				//アルファ
	Vector3 color;
	std::string textureFilename;//テクスチャファイル名
	std::unique_ptr<Texture> tex = std::make_unique<Texture>();

	Material() {
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
		color = { 1.0f,1.0f,1.0f };
	}
};