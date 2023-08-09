#pragma once
#include <string>
#include <Vector3.h>
#include <vector>
#include <unordered_map>
#include "Obj.h"
#include "LevelData.h"

//�O���̃��C�u�����ŏo���x���͖���
#pragma warning (push)
#pragma warning ( disable : 26800)
#include <json.hpp>
#pragma warning (pop)

class LevelLoader
{
public:
	void Load(const std::string &filename,const std::string& handle,int32_t stageNum);

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
