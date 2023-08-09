#pragma once
#include <string>
#include <Vector3.h>
#include <vector>
#include <unordered_map>
#include "Obj.h"
#include "LevelData.h"

//外部のライブラリで出た警告は無視
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
	//オブジェクトデータからObj3dに変換する関数
	static void SetObjectData(Obj3d& exportData, const LevelData::ObjectData& inportData);
};
