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

class Model : public VertexData
{
public:

	std::vector<Vertex> vertices;	//�����炪�撣���model�ǂݍ��݂���������s
	std::vector<uint16_t> indices;

	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;

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
	void CreateModel(const std::string t,bool smoothing = false);

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
	Model darumaM;
	Model firewispM;
	Model firewispSmoothingM;
	Model playerM;
	Model beetleM;
	Model subDevM;
	Model subDevM2;

	void PreLoad();

	static ModelManager* GetInstance() {
		static ModelManager instance;
		return &instance;
	};
	
};