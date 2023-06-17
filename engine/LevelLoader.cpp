#include "LevelLoader.h"
#include <fstream>
#include <cassert>

void LevelLoader::Load(const std::string& filename, const std::string& handle)
{
	const std::string fullpath = "Resources/" + filename + ".json";

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}
	
	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//�������t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name =
		deserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);
	
	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData levelData;

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		ObjectLoad(levelData, object);
	}

	LevelDatas[handle] = levelData;
}

LevelData* LevelLoader::GetData(const std::string& handle)
{
	return &LevelDatas[handle];
}

void LevelLoader::ObjectLoad(LevelData& levelData,nlohmann::json& object)
{
	//"type"�f�[�^���Ȃ���Εs���Ƃ���
	assert(object.contains("type"));

	//��ʂ��擾
	std::string type = object["type"].get<std::string>();

	//"MESH"�Ȃ�
	if (type.compare("MESH") == 0)
	{
		//�v�f�ǉ�
		levelData.mObjects.emplace_back(LevelData::ObjectData{});
		//���ǉ������v�f�̎Q�Ƃ𓾂�
		LevelData::ObjectData& objectData = levelData.mObjects.back();

		if (object.contains("file_name")) {
			objectData.fileName = object["file_name"];
		}

		if (object.contains("spawnpoint")) {
			objectData.spawnpointName = object["spawnpoint"];
		}

		//�p�����[�^�ǂݍ���
		nlohmann::json& transform = object["transform"];

		//���W
		//blender��directX�ł͍��W�n���قȂ邽�߁A�����ŕϊ�����(�G�N�X�|�[�g���ɕϊ�����̂���ʓI�ł͂���)
		objectData.translation.x = (float)transform["translation"][1];
		objectData.translation.y = (float)transform["translation"][2];
		objectData.translation.z = -1 * (float)transform["translation"][0];

		//��]
		objectData.rotation.x = -1 * (float)transform["rotation"][1];
		objectData.rotation.y = -1 * (float)transform["rotation"][2];
		objectData.rotation.z = (float)transform["rotation"][0];

		//�X�P�[�����O
		objectData.scaling.x = (float)transform["scaling"][1];
		objectData.scaling.y = (float)transform["scaling"][2];
		objectData.scaling.z = (float)transform["scaling"][0];

		//collider
		nlohmann::json& collider = object["collider"];
		if (collider != nullptr)
		{
			objectData.collider.have = true;
			//���S�_
			objectData.collider.center.x = (float)collider["center"][1];
			objectData.collider.center.y = (float)collider["center"][2];
			objectData.collider.center.z = (float)collider["center"][0];
			//�傫��
			objectData.collider.size.x = (float)collider["size"][1];
			objectData.collider.size.y = (float)collider["size"][2];
			objectData.collider.size.z = (float)collider["size"][0];
		}

		//�q�m�[�h������Ȃ�ċA
		//reverve���ă��������m�ۂ��Ȃ��ƁA���������ړ�����댯������
		if (object.contains("children"))
		{
			//reservenum++;
			//levelData->objects.reserve(reservenum);
			for (nlohmann::json& object_ : object["children"])
			{
				ObjectLoad(levelData, object_);
			}
		}
	}
}
