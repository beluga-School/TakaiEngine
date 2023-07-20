#pragma once
#include <string>
#include <Vector3.h>
#include <vector>
#include <unordered_map>
#include "Obj.h"

//�O���̃��C�u�����ŏo���x���͖���
#pragma warning (push)
#pragma warning ( disable : 26800)
#include <json.hpp>
#pragma warning (pop)

struct LevelData
{
	struct ColliderData {
		Vector3 center;
		Vector3 size;
		bool have = false;
	};
	struct ObjectData {
		std::string fileName;
		std::string spawnpointName;
		std::string eventtrigerName;
		std::string setObjectName;
	
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;

		ColliderData collider;
	};

	std::vector<ObjectData> mObjects;

	bool isCreate = false;

	std::string mHandle = "";
};

class LevelLoader
{
public:
	void Load(const std::string &filename,const std::string& handle);

	LevelData* GetData(const std::string& handle);
	std::unordered_map<std::string, LevelData> GetDataMap() {
		return LevelDatas;
	}

	static LevelLoader* Get()
	{
		static LevelLoader instance;
		return &instance;
	}

private:
	void ObjectLoad(LevelData& levelData, nlohmann::json& object);

	int32_t mReservenum = 0;

	LevelLoader(){};
	~LevelLoader(){};

	std::unordered_map<std::string, LevelData> LevelDatas;
};

class LevelDataExchanger
{
public:
	//�I�u�W�F�N�g�f�[�^����Obj3d�ɕϊ�����֐�
	static void SetObjectData(Obj3d& exportData, const LevelData::ObjectData& inportData);
};
