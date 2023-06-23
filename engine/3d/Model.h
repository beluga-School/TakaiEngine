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
	//TODO:mMeshes�����ōςނ悤�ɉ��P������
	//Obj�p���b�V��
	Mesh mMesh;

	//gltf�p���b�V��
	std::vector<Mesh> mMeshes;

	std::unordered_map<uint16_t, std::vector<uint16_t>> mSmoothData;

	Material mMaterial;

	/// <summary>
	/// �v���O�����ō쐬�������f���𐶐�����p�̊֐�
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateDefaultModel();

	/// <summary>
	/// ���f����ǂݍ���Ő�������֐�
	/// </summary>
	/// <param name="dx12_"></param>
	void CreateModel(const std::string modelname,bool smoothing = false);
	
	void CreateModelAssimp(const std::wstring& filename);

	std::string mSaveModelname = "";

	//���[�h�̐݌v��AGetModel�ŎQ�Ƃ������ɁA���[�h���ĂȂ��Ă����g������Ă��܂��̂ŁA
	//�܂��������I����ĂȂ����f�����Q�Ƃ�������nullptr��Ԃ����߁A������������ۑ����Ă���
	bool mCreated = false;

private:
	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
};

class ModelManager
{
public:
	void PreLoad();

	static void LoadModel(const std::string filepath, const std::string handle, bool smooth = false);
	static void LoadModelAssimp(const std::wstring filepath, const std::string handle);

	static Model* GetModel(const std::string handle);

	static ModelManager* Get() {
		static ModelManager instance;
		return &instance;
	};

private:
	static std::map<std::string, Model> sModels;

	ModelManager(){};
	~ModelManager(){};
};