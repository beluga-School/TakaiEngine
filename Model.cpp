#include "Model.h"
using namespace std;
#include "StringUtil.h"

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

void Model::CreateModel(const std::string t)
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
			}
		}
	}
	file.close();

	CreateVertex(vertices, indices);
}

Cube::Cube()
{
	vertices = {
		// x	 y	   z	 n	 u	   v
		//前
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
		{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,0.0f}},	//左上
		{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,0.0f}},	//右上
		//後ろ			 
		{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,1.0f}},	//左下
		{{-5.0f, 5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
		{{ 5.0f,-5.0f, 5.0f},{} ,{1.0f,1.0f}},	//右下
		{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
		//左				
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
		{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
		{{-5.0f, 5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
		{{-5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
		//右				
		{{ 5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
		{{ 5.0f,-5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
		{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
		//下					
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
		{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
		{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
		{{ 5.0f,-5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
		//上				
		{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
		{{-5.0f, 5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
		{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
	};

	indices =
	{
		//前
		0,1,2,		//三角形1つ目
		2,1,3,		//三角形2つ目
		//後ろ(前の面に4加算)
		6,5,4,		//三角形4つ目
		7,5,6,		//三角形3つ目
					//左
		8,9,10,		//三角形5つ目
		10,9,11,	//三角形6つ目
		//右
		14,13,12,	//三角形7つ目
		15,13,14,	//三角形8つ目
		//下
		19,17,18,	//三角形10つ目
		18,17,16,	//三角形9つ目
		//上
		20,21,22,	//三角形11つ目
		22,21,23,	//三角形12つ目

		//三角形を2つ合わせて四角にしてるので
		//6面にするなら12個必要...ってｺﾄ!? ﾜ｡｡｡
	};
}

Plate::Plate()
{
	vertices = {
		// x	 y	   z	 n	 u	   v
		//前
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
		{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,0.0f}},	//左上
		{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,0.0f}},	//右上
		//後ろ			 
		{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,1.0f}},	//左下
		{{-5.0f, 5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
		{{ 5.0f,-5.0f, 5.0f},{} ,{1.0f,1.0f}},	//右下
		{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
	};

	indices =
	{
		//前
		0,1,2,		//三角形1つ目
		2,1,3,		//三角形2つ目
		//後ろ(前の面に4加算)
		6,5,4,		//三角形4つ目
		7,5,6,		//三角形3つ目
	};
}

Triangle::Triangle()
{
	vertices = {
		// x	 y	   z	 n	 u	   v
		//前
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
		//{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,0.0f}},	//左上
		{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,0.0f}},	//右上
	};

	indices =
	{
		//前
		0,2,1,		//三角形1つ目
		//後ろ(前の面に4加算)
		1,2,0,		//三角形2つ目
	};
}

Line::Line()
{
	float xw = 0.1f;
	float yw = 0.1f;
	float zw = 100.0f;
	float zh = 100.0f;

	vertices = {
		// x	 y	   z	 n	 u	   v
		//前
		{{-xw,-yw,-zw},{} ,{0.0f,1.0f}},	//左下
		{{-xw, yw,-zw},{} ,{0.0f,0.0f}},	//左上
		{{ xw,-yw,-zw},{} ,{1.0f,1.0f}},	//右下
		{{ xw, yw,-zw},{} ,{1.0f,0.0f}},	//右上
		//後ろ			 
		{{-xw,-yw, zh},{} ,{0.0f,1.0f}},	//左下
		{{-xw, yw, zh},{} ,{0.0f,0.0f}},	//左上
		{{ xw,-yw, zh},{} ,{1.0f,1.0f}},	//右下
		{{ xw, yw, zh},{} ,{1.0f,0.0f}},	//右上
		//左				
		{{-xw,-yw,-zw},{} ,{0.0f,1.0f}},	//左下
		{{-xw,-yw, zh},{} ,{0.0f,0.0f}},	//左上
		{{-xw, yw,-zw},{} ,{1.0f,1.0f}},	//右下
		{{-xw, yw, zh},{} ,{1.0f,0.0f}},	//右上
		//右				
		{{ xw,-yw,-zw},{} ,{0.0f,1.0f}},	//左下
		{{ xw,-yw, zh},{} ,{0.0f,0.0f}},	//左上
		{{ xw, yw,-zw},{} ,{1.0f,1.0f}},	//右下
		{{ xw, yw, zh},{} ,{1.0f,0.0f}},	//右上
		//下					
		{{-xw,-yw,-zw},{} ,{0.0f,1.0f}},	//左下
		{{-xw,-yw, zh},{} ,{0.0f,0.0f}},	//左上
		{{ xw,-yw,-zw},{} ,{1.0f,1.0f}},	//右下
		{{ xw,-yw, zh},{} ,{1.0f,0.0f}},	//右上
		//上				
		{{-xw, yw,-zw},{} ,{0.0f,1.0f}},	//左下
		{{-xw, yw, zh},{} ,{0.0f,0.0f}},	//左上
		{{ xw, yw,-zw},{} ,{1.0f,1.0f}},	//右下
		{{ xw, yw, zh},{} ,{1.0f,0.0f}},	//右上
	};

	indices =
	{
		//前
		0,1,2,		//三角形1つ目
		2,1,3,		//三角形2つ目
		//後ろ(前の面に4加算)
		6,5,4,		//三角形4つ目
		7,5,6,		//三角形3つ目
					//左
		8,9,10,		//三角形5つ目
		10,9,11,	//三角形6つ目
		//右
		14,13,12,	//三角形7つ目
		15,13,14,	//三角形8つ目
		//下
		19,17,18,	//三角形10つ目
		18,17,16,	//三角形9つ目
		//上
		20,21,22,	//三角形11つ目
		22,21,23,	//三角形12つ目

		//三角形を2つ合わせて四角にしてるので
		//6面にするなら12個必要...ってｺﾄ!? ﾜ｡｡｡
	};
}

void ModelManager::PreLoad()
{
	cubeM.CreateModel("Cube");
	skyDomeM.CreateModel("skydome");
	boardM.CreateModel("board");
	//triangleM.CreateModel("triangle_mat");
}
