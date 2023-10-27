#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <string>
#include <list>

/*! LevelData
	blenderアドオンから出力したjsonファイルからステージ情報
*/
struct LevelData
{
	struct ColliderData {
		Vector3 center;
		Vector3 size;
		bool have = false;
	};
	struct ObjectData {
		std::string fileName;
		std::string textureName;
		std::string eventtrigerName;
		std::string setObjectName;
		
		Vector2 tiling;

		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;

		ColliderData collider;
	};

	std::vector<ObjectData> mObjects;

	bool isCreate = false;

	std::string mHandle = "";

	//このステージが何番目かを表す値(starなどのステージごとの収集状況の合致に使用)
	int32_t mStageNum = -1;

	//このステージがクリアされたか
	bool isClear = false;
};

