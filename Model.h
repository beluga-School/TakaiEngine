#pragma once
#include <vector>
#include "Vertex.h"


class Model : public VertexData
{
public:

	std::vector<Vertex> vertices;	//‚±‚¢‚Â‚ç‚ªŠæ’£‚ê‚Îmodel“Ç‚İ‚İ‚à‚¢‚¯‚é‚Áƒs
	std::vector<uint16_t> indices;
	void CreateModel(DirectX12 dx12_);

};


class Cube : public Model
{
public:
	Cube();
	~Cube(){};

	//void CreateModel(DirectXInit dx12_);
};

class Sprite : public Model
{
public:
	Sprite();
	~Sprite() {};
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