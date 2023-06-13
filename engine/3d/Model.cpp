#include "Model.h"
using namespace std;
#include "StringUtil.h"
#include <Vector2.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

std::map<std::string, Model> ModelManager::models;

void Model::CreateDefaultModel()
{
	CreateVertex(mesh.vertices, mesh.indices);
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);

	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);
		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�̃^�u�����͖�������
		if (key[0] == '\t') {
			key.erase(key.begin());//�擪�̕������폜
		}

		//�擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl") {
			//�}�e���A����
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
	//�t�@�C�������
	file.close();
}

void Model::CreateModel(const std::string t, bool smoothing)
{
	//obj�t�@�C�����J��
	std::ifstream file;
	const string modelname = t;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";
	file.open(directoryPath + filename);

	saveModelname = modelname;

	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	vector<Vector3> positions;	//���_�f�[�^
	vector<Vector3> normals;	//�@���x�N�g��
	vector<Vector2> texcoords;	//�e�N�X�`��uv



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
			//�}�e���A���̃t�@�C�����ǂݍ���
			string filename;
			line_stream >> filename;

			//�}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}

		if (key == "v")
		{
			//X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}
		if (key == "vt")
		{
			//U,V�����ǂݍ���
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		if (key == "vn")
		{
			//X,Y,Z�����ǂݍ���
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		if (key == "f")
		{
			string index_string;
			while (getline(line_stream,index_string,' '))
			{
				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ�
				istringstream index_stream(index_string);
				uint16_t indexPosition, indexTexcoord, indexNormal;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//�X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//�X���b�V�����΂�
				index_stream >> indexNormal;

				Vertex vertex{};
				vertex.pos.x = positions[indexPosition - 1].x;
				vertex.pos.y = positions[indexPosition - 1].y;
				vertex.pos.z = positions[indexPosition - 1].z;

				vertex.normal.x = normals[indexNormal - 1].x;
				vertex.normal.y = normals[indexNormal - 1].y;
				vertex.normal.z = normals[indexNormal - 1].z;
				
				vertex.uv.x = texcoords[indexTexcoord - 1].x;
				vertex.uv.y = texcoords[indexTexcoord - 1].y;
				
				mesh.vertices.emplace_back(vertex);

				//���_�C���f�b�N�X�ɒǉ�
				mesh.indices.emplace_back((uint16_t)mesh.indices.size());

				//�����Ńf�[�^��ێ�
				if (smoothing)
				{
					smoothData[indexPosition].emplace_back((uint16_t)mesh.vertices.size() - 1);
				}
			}
		}
	}
	file.close();

	for (int32_t i = 0; i < mesh.indices.size() / 3; i++)
	{	//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɂ����
		uint16_t indices0 = mesh.indices[i * 3 + 0];
		uint16_t indices1 = mesh.indices[i * 3 + 1];
		uint16_t indices2 = mesh.indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&mesh.vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&mesh.vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&mesh.vertices[indices2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&mesh.vertices[indices0].normal, normal);
		XMStoreFloat3(&mesh.vertices[indices1].normal, normal);
		XMStoreFloat3(&mesh.vertices[indices2].normal, normal);
	}

	if (smoothing)
	{
		//�����ŕێ������f�[�^���g���ăX���[�W���O���v�Z
		for (auto itr = smoothData.begin(); itr != smoothData.end(); ++itr)
		{
			std::vector<uint16_t>& v = itr->second;

			XMVECTOR normal = {};
			for (uint16_t index : v)
			{
				normal += XMVectorSet(mesh.vertices[index].normal.x, mesh.vertices[index].normal.y, mesh.vertices[index].normal.z, 0);
			}

			normal = XMVector3Normalize(normal / (float)v.size());

			for (uint16_t index : v)
			{
				mesh.vertices[index].normal = { normal.m128_f32[0],normal.m128_f32[1] ,normal.m128_f32[2] };
			}
		}
	}

	CreateVertex(mesh.vertices, mesh.indices);
}

void ModelManager::PreLoad()
{
	/*cubeM.CreateModel("Cube",true);
	skyDomeM.CreateModel("skydome");
	boardM.CreateModel("board");
	darumaM.CreateModel("boss");
	firewispM.CreateModel("firewisp");
	firewispSmoothingM.CreateModel("firewisp",true);
	playerM.CreateModel("player");
	subDevM.CreateModel("subDev");
	sphereM.CreateModel("Sphere",true);
	triangleM.CreateModel("triangle");
	beetleM.CreateModel("beetle");*/
	//beetleAss.AssimpLoader("beetle");

	LoadModel("Cube","Cube");
	LoadModel("skydome","skydome");
	LoadModel("board","board");
	LoadModel("boss","boss");
	LoadModel("firewisp","firewisp");
	LoadModel("player","player");
	LoadModel("subDev","subDev");
	LoadModel("Sphere","Sphere");
	LoadModel("triangle","triangle");
	LoadModel("beetle","beetle");
}

void ModelManager::LoadModel(const std::string filepath, const std::string handle, bool smooth)
{
	models[handle].CreateModel(filepath, smooth);
}

Model* ModelManager::GetModel(const std::string handle)
{
	return &models[handle];
}
