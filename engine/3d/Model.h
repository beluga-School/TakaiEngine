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

struct Mesh
{
	std::vector<Vertex> vertices;	//���_�f�[�^�z��
	std::vector<uint16_t> indices;	//�C���f�b�N�X�z��
	std::wstring diffuseMap;		//�e�N�X�`���̃t�@�C��
};

class Model : public VertexData
{
public:
	Mesh mesh;

	std::unordered_map<uint16_t, std::vector<uint16_t>> smoothData;

	Material material;

	/*ConstBuffer<ConstBufferDataB1> constBufferMaterial;*/

	//void LoadMesh(aiNode* node,const aiScene* scene);

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

	std::string saveModelname = "";

private:
	DirectX12* dx12 = DirectX12::Get();
};

class ModelManager
{
public:
	/*Model cubeM;
	Model skyDomeM;
	Model triangleM;
	Model boardM;
	Model darumaM;
	Model firewispM;
	Model firewispSmoothingM;
	Model playerM;
	Model beetleM;
	Model subDevM;
	Model sphereM;*/

	void PreLoad();

	static void LoadModel(const std::string filepath, const std::string handle, bool smooth = false);

	static Model* GetModel(const std::string handle);

	static ModelManager* Get() {
		static ModelManager instance;
		return &instance;
	};

private:
	static std::map<std::string, Model> models;

	ModelManager(){};
	~ModelManager(){};
};