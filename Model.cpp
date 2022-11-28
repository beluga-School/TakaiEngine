#include "Model.h"
using namespace std;
#include "StringUtil.h"

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

void Model::CreateModel(const std::string t)
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
			}
		}
	}
	file.close();

	CreateVertex(vertices, indices);
}

Cube::Cube()
{
	vertices = {
		// x	 y	   z	 n	 u	   v
		//�O
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//����
		{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,0.0f}},	//����
		{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,0.0f}},	//�E��
		//���			 
		{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,1.0f}},	//����
		{{-5.0f, 5.0f, 5.0f},{} ,{0.0f,0.0f}},	//����
		{{ 5.0f,-5.0f, 5.0f},{} ,{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//�E��
		//��				
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//����
		{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,0.0f}},	//����
		{{-5.0f, 5.0f,-5.0f},{} ,{1.0f,1.0f}},	//�E��
		{{-5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//�E��
		//�E				
		{{ 5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//����
		{{ 5.0f,-5.0f, 5.0f},{} ,{0.0f,0.0f}},	//����
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//�E��
		//��					
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//����
		{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,0.0f}},	//����
		{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//�E��
		{{ 5.0f,-5.0f, 5.0f},{} ,{1.0f,0.0f}},	//�E��
		//��				
		{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,1.0f}},	//����
		{{-5.0f, 5.0f, 5.0f},{} ,{0.0f,0.0f}},	//����
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//�E��
	};

	indices =
	{
		//�O
		0,1,2,		//�O�p�`1��
		2,1,3,		//�O�p�`2��
		//���(�O�̖ʂ�4���Z)
		6,5,4,		//�O�p�`4��
		7,5,6,		//�O�p�`3��
					//��
		8,9,10,		//�O�p�`5��
		10,9,11,	//�O�p�`6��
		//�E
		14,13,12,	//�O�p�`7��
		15,13,14,	//�O�p�`8��
		//��
		19,17,18,	//�O�p�`10��
		18,17,16,	//�O�p�`9��
		//��
		20,21,22,	//�O�p�`11��
		22,21,23,	//�O�p�`12��

		//�O�p�`��2���킹�Ďl�p�ɂ��Ă�̂�
		//6�ʂɂ���Ȃ�12�K�v...���ĺ�!? ܡ��
	};
}

Plate::Plate()
{
	vertices = {
		// x	 y	   z	 n	 u	   v
		//�O
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//����
		{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,0.0f}},	//����
		{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,0.0f}},	//�E��
		//���			 
		{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,1.0f}},	//����
		{{-5.0f, 5.0f, 5.0f},{} ,{0.0f,0.0f}},	//����
		{{ 5.0f,-5.0f, 5.0f},{} ,{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//�E��
	};

	indices =
	{
		//�O
		0,1,2,		//�O�p�`1��
		2,1,3,		//�O�p�`2��
		//���(�O�̖ʂ�4���Z)
		6,5,4,		//�O�p�`4��
		7,5,6,		//�O�p�`3��
	};
}

Triangle::Triangle()
{
	vertices = {
		// x	 y	   z	 n	 u	   v
		//�O
		{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//����
		//{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,0.0f}},	//����
		{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,0.0f}},	//�E��
	};

	indices =
	{
		//�O
		0,2,1,		//�O�p�`1��
		//���(�O�̖ʂ�4���Z)
		1,2,0,		//�O�p�`2��
	};
}

Line::Line()
{
	float xw = 0.1f;
	float yw = 0.1f;
	float zw = 100.0f;
	float zh = 100.0f;

	vertices = {
		// x	 y	   z	 n	 u	   v
		//�O
		{{-xw,-yw,-zw},{} ,{0.0f,1.0f}},	//����
		{{-xw, yw,-zw},{} ,{0.0f,0.0f}},	//����
		{{ xw,-yw,-zw},{} ,{1.0f,1.0f}},	//�E��
		{{ xw, yw,-zw},{} ,{1.0f,0.0f}},	//�E��
		//���			 
		{{-xw,-yw, zh},{} ,{0.0f,1.0f}},	//����
		{{-xw, yw, zh},{} ,{0.0f,0.0f}},	//����
		{{ xw,-yw, zh},{} ,{1.0f,1.0f}},	//�E��
		{{ xw, yw, zh},{} ,{1.0f,0.0f}},	//�E��
		//��				
		{{-xw,-yw,-zw},{} ,{0.0f,1.0f}},	//����
		{{-xw,-yw, zh},{} ,{0.0f,0.0f}},	//����
		{{-xw, yw,-zw},{} ,{1.0f,1.0f}},	//�E��
		{{-xw, yw, zh},{} ,{1.0f,0.0f}},	//�E��
		//�E				
		{{ xw,-yw,-zw},{} ,{0.0f,1.0f}},	//����
		{{ xw,-yw, zh},{} ,{0.0f,0.0f}},	//����
		{{ xw, yw,-zw},{} ,{1.0f,1.0f}},	//�E��
		{{ xw, yw, zh},{} ,{1.0f,0.0f}},	//�E��
		//��					
		{{-xw,-yw,-zw},{} ,{0.0f,1.0f}},	//����
		{{-xw,-yw, zh},{} ,{0.0f,0.0f}},	//����
		{{ xw,-yw,-zw},{} ,{1.0f,1.0f}},	//�E��
		{{ xw,-yw, zh},{} ,{1.0f,0.0f}},	//�E��
		//��				
		{{-xw, yw,-zw},{} ,{0.0f,1.0f}},	//����
		{{-xw, yw, zh},{} ,{0.0f,0.0f}},	//����
		{{ xw, yw,-zw},{} ,{1.0f,1.0f}},	//�E��
		{{ xw, yw, zh},{} ,{1.0f,0.0f}},	//�E��
	};

	indices =
	{
		//�O
		0,1,2,		//�O�p�`1��
		2,1,3,		//�O�p�`2��
		//���(�O�̖ʂ�4���Z)
		6,5,4,		//�O�p�`4��
		7,5,6,		//�O�p�`3��
					//��
		8,9,10,		//�O�p�`5��
		10,9,11,	//�O�p�`6��
		//�E
		14,13,12,	//�O�p�`7��
		15,13,14,	//�O�p�`8��
		//��
		19,17,18,	//�O�p�`10��
		18,17,16,	//�O�p�`9��
		//��
		20,21,22,	//�O�p�`11��
		22,21,23,	//�O�p�`12��

		//�O�p�`��2���킹�Ďl�p�ɂ��Ă�̂�
		//6�ʂɂ���Ȃ�12�K�v...���ĺ�!? ܡ��
	};
}

void ModelManager::PreLoad()
{
	cubeM.CreateModel("Cube");
	skyDomeM.CreateModel("skydome");
	boardM.CreateModel("board");
	//triangleM.CreateModel("triangle_mat");
}
