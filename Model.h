#pragma once
#include <vector>
#include "Vertex.h"

#include <fstream>
#include <sstream>
#include <string>

class Model : public VertexData
{
public:

	std::vector<Vertex> vertices;	//�����炪�撣���model�ǂݍ��݂���������s
	std::vector<uint16_t> indices;

	/// <summary>
	/// �v���O�����ō쐬�������f���𐶐�����p�̊֐�
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateDefaultModel(DirectX12 dx12_);

	/// <summary>
	/// ���f����ǂݍ���Ő�������֐�
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