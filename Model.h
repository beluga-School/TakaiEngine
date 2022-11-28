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

class Model : public VertexData
{
public:

	std::vector<Vertex> vertices;	//こいつらが頑張ればmodel読み込みもいけるっピ
	std::vector<uint16_t> indices;

	Material material;
	/*ConstBuffer<ConstBufferDataB1> constBufferMaterial;*/

	/// <summary>
	/// プログラムで作成したモデルを生成する用の関数
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateDefaultModel();

	/// <summary>
	/// モデルを読み込んで生成する関数
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateModel(const std::string t);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

private:
	DirectX12* dx12 = DirectX12::GetInstance();
};

class ModelManager
{
public:
	Model cubeM;
	Model skyDomeM;
	Model triangleM;
	Model boardM;

	void PreLoad();

	static ModelManager* GetInstance() {
		static ModelManager instance;
		return &instance;
	};
	
};


class Cube : public Model
{
public:
	Cube();
	~Cube(){};
};

class Plate : public Model
{
public:
	Plate();
	~Plate() {};
};

class Triangle : public Model
{
public:
	Triangle();
	~Triangle() {};
};

class Line : public Model
{
public:
	Line();
	~Line() {};
};