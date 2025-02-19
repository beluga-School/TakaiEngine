#include "Model.h"
#include "StringUtil.h"
#include <Vector2.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <AssimpLoader.h>

using namespace std;
using namespace DirectX;

std::map<std::string, Model> ModelManager::sModels;

void Model::CreateDefaultModel()
{
	CreateVertex(mMesh.vertices, mMesh.indices);
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);
		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin());//先頭の文字を削除
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl") {
			//マテリアル名
			line_stream >> mMaterial.mName;
		}

		if (key == "Ka") {
			line_stream >> mMaterial.mAmbient.x;
			line_stream >> mMaterial.mAmbient.y;
			line_stream >> mMaterial.mAmbient.z;
		}

		if (key == "Kd") {
			line_stream >> mMaterial.mDiffuse.x;
			line_stream >> mMaterial.mDiffuse.y;
			line_stream >> mMaterial.mDiffuse.z;
		}

		if (key == "Ks") {
			line_stream >> mMaterial.mSpecular.x;
			line_stream >> mMaterial.mSpecular.y;
			line_stream >> mMaterial.mSpecular.z;
		}

		if (key == "map_Kd")
		{
			line_stream >> mMaterial.mTextureFilename;

			mMaterial.mTexture->Load(*ConvertStringToWChar(directoryPath + mMaterial.mTextureFilename).c_str());
		}
	}
	//ファイルを閉じる
	file.close();
}

void Model::CreateModel(const std::string modelname, bool smoothing)
{
	//objファイルを開く
	std::ifstream file;
	const string modelname_ = modelname;
	const string filename = modelname_ + ".obj";
	const string directoryPath = "Resources/" + modelname_ + "/";
	file.open(directoryPath + filename);

	mSaveModelname = modelname_;

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	vector<Vector3> positions;	//頂点データ
	vector<Vector3> normals;	//法線ベクトル
	vector<Vector2> texcoords;	//テクスチャuv

	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key,' ');

		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			string matfilename;
			line_stream >> matfilename;

			//マテリアル読み込み
			LoadMaterial(directoryPath, matfilename);
		}

		if (key == "v")
		{
			//X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//座標データに追加
			positions.emplace_back(position);
		}
		if (key == "vt")
		{
			//U,V成分読み込み
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		if (key == "vn")
		{
			//X,Y,Z成分読み込み
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		if (key == "f")
		{
			string index_string;
			while (getline(line_stream,index_string,' '))
			{
				//頂点インデックス1個分の文字列をストリームに変換
				istringstream index_stream(index_string);
				uint16_t indexPosition, indexTexcoord, indexNormal;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexNormal;

				Vertex vertex{};
				vertex.pos.x = positions[indexPosition - 1].x;
				vertex.pos.y = positions[indexPosition - 1].y;
				vertex.pos.z = positions[indexPosition - 1].z;

				vertex.normal.x = normals[indexNormal - 1].x;
				vertex.normal.y = normals[indexNormal - 1].y;
				vertex.normal.z = normals[indexNormal - 1].z;
				
				vertex.uv.x = texcoords[indexTexcoord - 1].x;
				vertex.uv.y = texcoords[indexTexcoord - 1].y;
				
				mMesh.vertices.emplace_back(vertex);

				//頂点インデックスに追加
				mMesh.indices.emplace_back(static_cast<uint16_t>(mMesh.indices.size()));

				//ここでデータを保持
				if (smoothing)
				{
					mSmoothData[indexPosition].emplace_back(static_cast<uint16_t>(mMesh.vertices.size() - 1));
				}
			}
		}
	}
	file.close();

	for (int32_t i = 0; i < mMesh.indices.size() / 3; i++)
	{	//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数にいれる
		uint16_t indices0 = mMesh.indices[i * 3 + 0];
		uint16_t indices1 = mMesh.indices[i * 3 + 1];
		uint16_t indices2 = mMesh.indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&mMesh.vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&mMesh.vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&mMesh.vertices[indices2].pos);

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化(長さを1にする)
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&mMesh.vertices[indices0].normal, normal);
		XMStoreFloat3(&mMesh.vertices[indices1].normal, normal);
		XMStoreFloat3(&mMesh.vertices[indices2].normal, normal);
	}

	if (smoothing)
	{
		//ここで保持したデータを使ってスムージングを計算
		for (auto itr = mSmoothData.begin(); itr != mSmoothData.end(); ++itr)
		{
			std::vector<uint16_t>& v = itr->second;

			XMVECTOR normal = {};
			for (uint16_t index : v)
			{
				normal += XMVectorSet(mMesh.vertices[index].normal.x, mMesh.vertices[index].normal.y, mMesh.vertices[index].normal.z, 0);
			}

			normal = XMVector3Normalize(normal / (float)v.size());

			for (uint16_t index : v)
			{
				mMesh.vertices[index].normal = { 
					normal.m128_f32[0],
					normal.m128_f32[1],
					normal.m128_f32[2] 
				};
			}
		}
	}

	CreateVertex(mMesh.vertices, mMesh.indices);

	mCreated = true;
}

void Model::CreateModelAssimp(const std::wstring& filename)
{
	//fbx読み込みの形跡
	ImportSettings importSetting =
	{
		filename.c_str(),
		mMeshes,
		false,
		false,
	};
	AssimpLoader loader;

	if (!loader.Load(importSetting))
	{
		assert(0);
	}

	for (size_t i = 0; i < mMeshes.size(); i++)
	{
		auto vertices = mMeshes[i].vertices;
		auto indices = mMeshes[i].indices;

		CreateVertex(vertices, indices);
		mMesh.vertices = vertices;
		mMesh.indices = indices;

		//mMaterial.mTextire->Load(*mMeshes[i].diffuseMap.c_str());
	}

	mCreated = true;
}

void ModelManager::PreLoad()
{
	LoadModel("Cube","Cube");
	LoadModel("Sphere","Sphere");
	LoadModel("skydome","skydome");
	LoadModel("firewisp","firewisp",true);
	LoadModel("beetle","beetle",true);
	LoadModel("plate","plate",true);
	LoadModel("spawnpoint", "spawnpoint");
	LoadModel("eventtriger", "eventtriger");
	LoadModel("groundCube", "groundCube", true);
	LoadModel("BlankCube", "BlankCube");
	LoadModel("Sphere", "smSphere", true);
	LoadModel("BlankSphere", "BlankSphere", true);
	LoadModel("ICOSphere", "ICOSphere");
	LoadModel("targetMark", "targetMark");
	LoadModel("GoalPaul", "GoalPaul",true);
	LoadModel("GoalFrag", "GoalFrag",true);
	LoadModel("star", "star",true);
	LoadModel("Cannon", "Cannon",true);
	LoadModel("dokan", "dokan",true);
	LoadModel("slime", "slime",true);
	LoadModel("Coin", "Coin",true);
	LoadModel("bombSolider", "bombSolider",true);
	LoadModel("smoke", "smoke",true);
	LoadModel("teiboku", "teiboku",true);
}

void ModelManager::LoadModel(const std::string filepath, const std::string handle, bool smooth)
{
	sModels[handle].CreateModel(filepath, smooth);
}

void ModelManager::LoadModelAssimp(const std::wstring filepath, const std::string handle)
{
	sModels[handle].CreateModelAssimp(filepath);
}

Model* ModelManager::GetModel(const std::string handle)
{
	if (sModels[handle].mCreated == false)
	{
		return nullptr;
	}
	return &sModels[handle];
}
