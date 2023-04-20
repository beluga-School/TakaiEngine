#include "Model.h"
using namespace std;
#include "StringUtil.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

bool Model::AssimpLoader(const std::string t)
{
	//�C���|�[�^�[�𐶐�
	Assimp::Importer importer;

	//�t�@�C������.obj��.mtl�����āA�����̖��O���t�@�C�����ƈ�v���Ă���ꍇ�̂ݓǂݍ���
	const string modelname = t;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";

	//assimp�Ńt�@�C���ǂݍ���(assimp�̌`��)
	const aiScene* scene = importer.ReadFile(
		directoryPath + filename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_ConvertToLeftHanded |
		aiProcess_FixInfacingNormals
	);

	//�_���Ȃ�_��
	if (scene == nullptr)
	{
		//importer.GetErrorString();
		return false;
	}

	//�����̓s���̂����`���ɂ����ŕϊ�����
	vector<XMFLOAT3> positions;	//���_�f�[�^
	vector<XMFLOAT3> normals;	//�@���x�N�g��
	vector<XMFLOAT2> texcoords;	//�e�N�X�`��uv

	UINT backIndex = 0;	//�C���f�b�N�X�𑫂�

	//���������̏������m�[�h���Ƃɂ��Ȃ��Ƃ����Ȃ����߁A�����Ńm�[�h���Ԃ��for������

	//���b�V�����Ƃɏ���ۑ�
	aiMesh* mesh = *scene->mMeshes;

	for (unsigned int k = 0; k < scene->mNumMeshes; k++)
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			//�ȉ�3��vertex�f�[�^(CreateModel��v,vt,n�œǂݍ���ł����Ƃ���)
			//���W
			aiVector3D vertex = mesh->mVertices[i];
			//�����������ĂȂ����ǃ��[���h�ϊ��s����|���Ȃ��Ɛ��������f����ǂݍ��߂Ȃ��炵���̂Ŋ|����
			//���Ԃ񂱂���mRootNode�́A���for���ɂ����Ƃ��Ƀm�[�h���Ƃ̂�ɂ��Ȃ��Ƃ����Ȃ�
			//���ƁA�Ȃ񂩎q�I�u�W�F�N�g�̍s����S���|���Ȃ��ƍs���Ȃ��炵���@���ꂪ��Ԃ킩���
			vertex *= scene->mRootNode->mTransformation;

			positions.emplace_back();
			positions.back().x = vertex.x;
			positions.back().y = vertex.y;
			positions.back().z = vertex.z;

			backIndex++;

			//�@��
			if (mesh->mNormals)
			{
				aiVector3D norm = mesh->mNormals[i];
				norm.Normalize();
				normals.emplace_back(norm.x, norm.y, norm.z);
			}

			//uv
			if (mesh->HasTextureCoords(0))
			{
				texcoords.push_back({
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
					});
			}

			vertices.emplace_back(Vertex{
				positions.back(),
				normals.back(),
				texcoords.back()
				});
		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			for (unsigned int i = 0; i < face.mNumIndices; i++)
			{
				UINT ind = face.mIndices[i];

				indices.emplace_back(ind + backIndex);
			}
		}
	}

	//Wrelf�搶�͂����ŏ�̏������܂Ƃ߂Ċ֐��ɂ��Ă�����
	//fNode(scene->mRootNode) //������

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		//�ȉ�5�̓}�e���A���f�[�^(LoadMaterial�œǂݍ���ł����Ƃ���)
		//�}�e���A����(����H�����Œ��ړǂݍ��ނ悤�ɂȂ�Ȃ炢��Ȃ�����)
		aiMaterial* mat = scene->mMaterials[i];

		aiString name;
		mat->Get(AI_MATKEY_NAME, name);
		material.name = name.C_Str();

		//�A���r�G���g
		aiColor3D ambient;
		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		material.ambient = { ambient.r,ambient.g,ambient.b };

		//�f�B�t���[�Y
		aiColor3D diffuse;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		material.diffuse = { diffuse.r,diffuse.g,diffuse.b };

		//�X�y�L�����[
		aiColor3D specular;
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		material.specular = { specular.r,specular.g,specular.b };

		//�}�e���A���̃e�N�X�`����
		aiString texname;
		
		int h = mat->GetTextureCount(aiTextureType_DIFFUSE);
		
		if (h)
		{
			mat->GetTexture(
				aiTextureType_DIFFUSE,
				mat->GetTextureCount(aiTextureType_DIFFUSE),
				&texname);
			material.textureFilename = texname.C_Str();
			
			//���_����1:material��textureFilename���������ǂݍ��߂ĂȂ�
			//�������@�̌����͂܂����ĂȂ�
			material.tex->Load(ConvertStringToWChar(directoryPath + material.textureFilename).c_str());
		}
		else
		{
			material.tex->Load(ConvertStringToWChar("Resources/default.png").c_str());
		}
	}


	//��������͓ǂ݂����������g���ăf�[�^���g����悤�ɂ��Ă���

	//���_����2:vertices�̐���4057�����Ȃ�
	//����������5169����͂�
	//�Ȃ̂ɑ���ĂȂ��̂ł����Ŕz��O���Q�Ƃ��ăG���[�ɂȂ�
	//�������@ children��������ۂ��̂ŁA�����ɑ���Ȃ�vertices�������Ă邩��
	//�Ƃ肠�����m�[�h�̐����񂷐݌v�ɂ��Ă݂�

	for (unsigned int i = 0; i < indices.size() / 3; i++)
	{	//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɂ����
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}

	//if (smoothing)
	//{
	//	//�����ŕێ������f�[�^���g���ăX���[�W���O���v�Z
	//	for (auto itr = smoothData.begin(); itr != smoothData.end(); ++itr)
	//	{
	//		std::vector<unsigned short>& v = itr->second;

	//		XMVECTOR normal = {};
	//		for (unsigned short index : v)
	//		{
	//			normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
	//		}

	//		normal = XMVector3Normalize(normal / (float)v.size());

	//		for (unsigned short index : v)
	//		{
	//			vertices[index].normal = { normal.m128_f32[0],normal.m128_f32[1] ,normal.m128_f32[2] };
	//		}
	//	}
	//}

	CreateVertex(vertices, indices);

	//������importer���E���Ă��ꂢ��
	return true;
}


void Model::CreateDefaultModel()
{
	CreateVertex( vertices, indices);
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
	std::ifstream file;
	//obj�t�@�C�����J��
	//file.open(t);
	const string modelname = t;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";
	file.open(directoryPath + filename);

	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	vector<XMFLOAT3> positions;	//���_�f�[�^
	vector<XMFLOAT3> normals;	//�@���x�N�g��
	vector<XMFLOAT2> texcoords;	//�e�N�X�`��uv



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
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}
		if (key == "vt")
		{
			//U,V�����ǂݍ���
			XMFLOAT2 texcoord{};
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
			XMFLOAT3 normal{};
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
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				//���_�C���f�b�N�X�ɒǉ�
				indices.emplace_back((unsigned short)indices.size());

				//�����Ńf�[�^��ێ�
				if (smoothing)
				{
					smoothData[indexPosition].emplace_back(vertices.size() - 1);
				}
			}
		}
	}
	file.close();

	for (int i = 0; i < indices.size() / 3; i++)
	{	//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɂ����
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}

	if (smoothing)
	{
		//�����ŕێ������f�[�^���g���ăX���[�W���O���v�Z
		for (auto itr = smoothData.begin(); itr != smoothData.end(); ++itr)
		{
			std::vector<unsigned short>& v = itr->second;

			XMVECTOR normal = {};
			for (unsigned short index : v)
			{
				normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
			}

			normal = XMVector3Normalize(normal / (float)v.size());

			for (unsigned short index : v)
			{
				vertices[index].normal = { normal.m128_f32[0],normal.m128_f32[1] ,normal.m128_f32[2] };
			}
		}
	}

	CreateVertex(vertices, indices);
}

void ModelManager::PreLoad()
{
	cubeM.CreateModel("Cube", true);
	skyDomeM.CreateModel("skydome");
	boardM.CreateModel("board");
	darumaM.CreateModel("boss");
	firewispM.CreateModel("firewisp");
	firewispSmoothingM.CreateModel("firewisp",true);
	playerM.CreateModel("player");
	subDevM.CreateModel("subDev");
	sphereM.CreateModel("Sphere",true);
	triangleM.CreateModel("triangle");
	beetleM.CreateModel("beetle");
	beetleAss.AssimpLoader("beetle");
}
