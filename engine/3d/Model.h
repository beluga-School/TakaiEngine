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

struct Triangle
{
	Vector3 pos0;
	Vector3 pos1;
	Vector3 pos2;

	Vector3 normal;
};

/*! Model
	3Dオブジェクトに使用するモデル
*/
class Model : public VertexData
{
public:
	/// <summary>
	/// プログラムで作成したモデルを生成する用の関数
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateDefaultModel();

	/// <summary>
	/// モデルを読み込んで生成する関数
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateModel(const std::string modelname,bool smoothing = false);
	
	/// <summary>
	/// assimpでモデルを作成
	/// </summary>
	/// <param name="filename"></param>
	void CreateModelAssimp(const std::wstring& filename);

public:
	//Obj用メッシュ
	Mesh mMesh;

	//gltf用メッシュ
	std::vector<Mesh> mMeshes;

	//三角形
	std::vector<Triangle> mTriangles;

	std::unordered_map<uint16_t, std::vector<uint16_t>> mSmoothData;

	Material mMaterial;

	std::string mSaveModelname = "";

	//ロードの設計上、GetModelで参照した時に、ロードしてなくても中身が作られてしまうので、
	//まだ生成が終わってないモデルを参照した時にnullptrを返すため、生成したかを保存しておく
	bool mCreated = false;
private:
	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
};

class ModelManager
{
public:
	//中で指定したモデルを起動時に読み込む
	void PreLoad();

	/// <summary>
	/// 通常読み込み
	/// </summary>
	/// <param name="filepath">読み込みファイルパス</param>
	/// <param name="handle">呼び出し時のハンドル</param>
	/// <param name="smooth">スムースをかけるか</param>
	static void LoadModel(const std::string filepath, const std::string handle, bool smooth = false);
	
	/// <summary>
	/// Assimp読み込み
	/// </summary>
	/// <param name="filepath">読み込みファイルパス</param>
	/// <param name="handle">呼び出し時のハンドル</param>
	static void LoadModelAssimp(const std::wstring filepath, const std::string handle);
	//ハンドルでモデル取得
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