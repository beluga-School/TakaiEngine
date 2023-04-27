#pragma once
#define NOMINMAX
#include <DirectXInit.h>
#include <string>
#include <vector>
#include "Model.h"

struct aiMesh;
struct aiMaterial;

struct ImportSettings
{
	const wchar_t* filename = nullptr;	//ファイルパス
	std::vector<Mesh>& meshes;			//出力先のメッシュ配列
	bool inverseU = false;				//U座標を反転させるか
	bool inverseV = false;				//V座標を反転させるか
};

class AssimpLoader
{
public:
	bool Load(ImportSettings setting);	//モデルのロード

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
	void LoadTexture(const wchar_t* filename,Mesh& dst,const aiMaterial* src);

};

