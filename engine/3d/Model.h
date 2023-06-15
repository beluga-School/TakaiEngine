#pragma once
#include <vector>
#include "Vertex.h"
#include "Vector3.h"

#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "ConstBuffer.h"
#include "Material.h"
#include <unordered_map>

struct Mesh
{
	std::vector<Vertex> vertices;	//頂点データ配列
	std::vector<uint16_t> indices;	//インデックス配列
	std::wstring diffuseMap;		//テクスチャのファイル
};

class Model : public VertexData
{
public:
	Mesh mMesh;

	std::unordered_map<uint16_t, std::vector<uint16_t>> mSmoothData;

	Material mMaterial;

	/*ConstBuffer<ConstBufferDataB1> constBufferMaterial;*/

	//void LoadMesh(aiNode* node,const aiScene* scene);

	/// <summary>
	/// プログラムで作成したモデルを生成する用の関数
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateDefaultModel();

	/// <summary>
	/// モデルを読み込んで生成する関数
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateModel(const std::string t,bool smoothing = false);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	std::string mSaveModelname = "";
};

class ModelManager
{
public:
	void PreLoad();

	static void LoadModel(const std::string filepath, const std::string handle, bool smooth = false);

	static Model* GetModel(const std::string handle);

	static ModelManager* Get() {
		static ModelManager instance;
		return &instance;
	};

private:
	static std::map<std::string, Model> sModels;

	ModelManager(){};
	~ModelManager(){};
};