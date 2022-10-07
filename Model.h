#pragma once
#include <vector>
#include "Vertex.h"

#include <fstream>
#include <sstream>
#include <string>

class Model : public VertexData
{
public:

	std::vector<Vertex> vertices;	//こいつらが頑張ればmodel読み込みもいけるっピ
	std::vector<uint16_t> indices;

	/// <summary>
	/// プログラムで作成したモデルを生成する用の関数
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateDefaultModel(DirectX12 dx12_);

	/// <summary>
	/// モデルを読み込んで生成する関数
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateModel(const wchar_t* t,DirectX12 dx12_);

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