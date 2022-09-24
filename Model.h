#pragma once
#include <vector>
#include "Vertex.h"


class Model : public VertexData
{
public:

	std::vector<Vertex> vertices;	//�����炪�撣���model�ǂݍ��݂���������s
	std::vector<uint16_t> indices;
	void CreateModel(DirectX12 dx12_);

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