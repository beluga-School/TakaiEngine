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

	std::vector<Vertex> vertices;	//�����炪�撣���model�ǂݍ��݂���������s
	std::vector<uint16_t> indices;

	Material material;
	/*ConstBuffer<ConstBufferDataB1> constBufferMaterial;*/

	/// <summary>
	/// �v���O�����ō쐬�������f���𐶐�����p�̊֐�
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateDefaultModel();

	/// <summary>
	/// ���f����ǂݍ���Ő�������֐�
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateModel(const std::string t);

	/// <summary>
	/// �}�e���A���ǂݍ���
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