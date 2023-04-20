#include "Model.h"
using namespace std;
#include "StringUtil.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

bool Model::AssimpLoader(const std::string t)
{
	//インポーターを生成
	Assimp::Importer importer;

	//ファイル内に.objと.mtlを入れて、それらの名前がファイル名と一致している場合のみ読み込み
	const string modelname = t;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";

	//assimpでファイル読み込み(assimpの形式)
	const aiScene* scene = importer.ReadFile(
		directoryPath + filename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_ConvertToLeftHanded |
		aiProcess_FixInfacingNormals
	);

	//ダメならダメ
	if (scene == nullptr)
	{
		//importer.GetErrorString();
		return false;
	}

	//自分の都合のいい形式にここで変換する
	vector<XMFLOAT3> positions;	//頂点データ
	vector<XMFLOAT3> normals;	//法線ベクトル
	vector<XMFLOAT2> texcoords;	//テクスチャuv

	UINT backIndex = 0;	//インデックスを足す

	//多分これらの処理をノードごとにしないといけないため、ここでノード分ぶん回すforが入る

	//メッシュごとに情報を保存
	aiMesh* mesh = *scene->mMeshes;

	for (unsigned int k = 0; k < scene->mNumMeshes; k++)
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			//以下3つはvertexデータ(CreateModelのv,vt,nで読み込んでいたところ)
			//座標
			aiVector3D vertex = mesh->mVertices[i];
			//何も理解してないけどワールド変換行列を掛けないと正しくモデルを読み込めないらしいので掛ける
			//たぶんここのmRootNodeは、後でfor文にしたときにノードごとのやつにしないといけない
			//あと、なんか子オブジェクトの行列も全部掛けないと行けないらしい　それが一番わからん
			vertex *= scene->mRootNode->mTransformation;

			positions.emplace_back();
			positions.back().x = vertex.x;
			positions.back().y = vertex.y;
			positions.back().z = vertex.z;

			backIndex++;

			//法線
			if (mesh->mNormals)
			{
				aiVector3D norm = mesh->mNormals[i];
				norm.Normalize();
				normals.emplace_back(norm.x, norm.y, norm.z);
			}

			//uv
			if (mesh->HasTextureCoords(0))
			{
				texcoords.push_back({
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
					});
			}

			vertices.emplace_back(Vertex{
				positions.back(),
				normals.back(),
				texcoords.back()
				});
		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			for (unsigned int i = 0; i < face.mNumIndices; i++)
			{
				UINT ind = face.mIndices[i];

				indices.emplace_back(ind + backIndex);
			}
		}
	}

	//Wrelf先生はここで上の処理をまとめて関数にしてあった
	//fNode(scene->mRootNode) //←これ

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		//以下5つはマテリアルデータ(LoadMaterialで読み込んでいたところ)
		//マテリアル名(いる？ここで直接読み込むようになるならいらないかも)
		aiMaterial* mat = scene->mMaterials[i];

		aiString name;
		mat->Get(AI_MATKEY_NAME, name);
		material.name = name.C_Str();

		//アンビエント
		aiColor3D ambient;
		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		material.ambient = { ambient.r,ambient.g,ambient.b };

		//ディフューズ
		aiColor3D diffuse;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		material.diffuse = { diffuse.r,diffuse.g,diffuse.b };

		//スペキュラー
		aiColor3D specular;
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		material.specular = { specular.r,specular.g,specular.b };

		//マテリアルのテクスチャ名
		aiString texname;
		
		int h = mat->GetTextureCount(aiTextureType_DIFFUSE);
		
		if (h)
		{
			mat->GetTexture(
				aiTextureType_DIFFUSE,
				mat->GetTextureCount(aiTextureType_DIFFUSE),
				&texname);
			material.textureFilename = texname.C_Str();
			
			//問題点その1:materialのtextureFilenameが正しく読み込めてない
			//解決方法の検討はまだついてない
			material.tex->Load(ConvertStringToWChar(directoryPath + material.textureFilename).c_str());
		}
		else
		{
			material.tex->Load(ConvertStringToWChar("Resources/default.png").c_str());
		}
	}


	//こっからは読みだした情報を使ってデータを使えるようにしていく

	//問題点その2:verticesの数が4057個しかない
	//正しい数は5169個あるはず
	//なのに足りてないのでここで配列外を参照してエラーになる
	//解決方法 childrenがあるっぽいので、そこに足りないverticesが入ってるかも
	//とりあえずノードの数分回す設計にしてみる

	for (unsigned int i = 0; i < indices.size() / 3; i++)
	{	//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数にいれる
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化(長さを1にする)
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}

	//if (smoothing)
	//{
	//	//ここで保持したデータを使ってスムージングを計算
	//	for (auto itr = smoothData.begin(); itr != smoothData.end(); ++itr)
	//	{
	//		std::vector<unsigned short>& v = itr->second;

	//		XMVECTOR normal = {};
	//		for (unsigned short index : v)
	//		{
	//			normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
	//		}

	//		normal = XMVector3Normalize(normal / (float)v.size());

	//		for (unsigned short index : v)
	//		{
	//			vertices[index].normal = { normal.m128_f32[0],normal.m128_f32[1] ,normal.m128_f32[2] };
	//		}
	//	}
	//}

	CreateVertex(vertices, indices);

	//ここでimporterを殺してきれいに
	return true;
}


void Model::CreateDefaultModel()
{
	CreateVertex( vertices, indices);
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
			line_stream >> material.name;
		}

		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}

		if (key == "Kd") {
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}

		if (key == "Ks") {
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}

		if (key == "map_Kd")
		{
			line_stream >> material.textureFilename;

			material.tex->Load(ConvertStringToWChar(directoryPath + material.textureFilename).c_str());
		}
	}
	//ファイルを閉じる
	file.close();
}

void Model::CreateModel(const std::string t, bool smoothing)
{
	std::ifstream file;
	//objファイルを開く
	//file.open(t);
	const string modelname = t;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";
	file.open(directoryPath + filename);

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	vector<XMFLOAT3> positions;	//頂点データ
	vector<XMFLOAT3> normals;	//法線ベクトル
	vector<XMFLOAT2> texcoords;	//テクスチャuv



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
			string filename;
			line_stream >> filename;

			//マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		if (key == "v")
		{
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//座標データに追加
			positions.emplace_back(position);
		}
		if (key == "vt")
		{
			//U,V成分読み込み
			XMFLOAT2 texcoord{};
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
			XMFLOAT3 normal{};
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
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				//頂点インデックスに追加
				indices.emplace_back((unsigned short)indices.size());

				//ここでデータを保持
				if (smoothing)
				{
					smoothData[indexPosition].emplace_back(vertices.size() - 1);
				}
			}
		}
	}
	file.close();

	for (int i = 0; i < indices.size() / 3; i++)
	{	//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数にいれる
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化(長さを1にする)
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}

	if (smoothing)
	{
		//ここで保持したデータを使ってスムージングを計算
		for (auto itr = smoothData.begin(); itr != smoothData.end(); ++itr)
		{
			std::vector<unsigned short>& v = itr->second;

			XMVECTOR normal = {};
			for (unsigned short index : v)
			{
				normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
			}

			normal = XMVector3Normalize(normal / (float)v.size());

			for (unsigned short index : v)
			{
				vertices[index].normal = { normal.m128_f32[0],normal.m128_f32[1] ,normal.m128_f32[2] };
			}
		}
	}

	CreateVertex(vertices, indices);
}

void ModelManager::PreLoad()
{
	cubeM.CreateModel("Cube", true);
	skyDomeM.CreateModel("skydome");
	boardM.CreateModel("board");
	darumaM.CreateModel("boss");
	firewispM.CreateModel("firewisp");
	firewispSmoothingM.CreateModel("firewisp",true);
	playerM.CreateModel("player");
	subDevM.CreateModel("subDev");
	sphereM.CreateModel("Sphere",true);
	triangleM.CreateModel("triangle");
	beetleM.CreateModel("beetle");
	beetleAss.AssimpLoader("beetle");
}
