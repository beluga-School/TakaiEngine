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
	const wchar_t* mFILENAME = nullptr;	//ファイルパス
	std::vector<Mesh>& mMeshes;			//出力先のメッシュ配列
	bool mInverseU = false;				//U座標を反転させるか
	bool mInverseV = false;				//V座標を反転させるか
};

/*! AssimpLoader
	Assimpでの読み込みを行うクラス
*/
class AssimpLoader
{
public:
	bool Load(const ImportSettings& setting);	//モデルのロード

private:
	//メッシュの読み込み
	void LoadMesh(Mesh& dst, const aiMesh* src, const bool& inverseU, const bool& inverseV);
	
	//テクスチャの読み込み
	void LoadTexture(const wchar_t* filename,Mesh& dst,const aiMaterial* src);
};

