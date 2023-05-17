#pragma once
#include <string>
#include <Vector3.h>
#include <vector>
#include <json.hpp>

struct LevelData
{
	struct ColliderData {
		Vector3 center;
		Vector3 size;
	};
	struct ObjectData {
		std::string fileName;
	
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;

		ColliderData collider;
	};

	std::vector<ObjectData> objects;
};

class LevelLoader
{
public:
	LevelData Load(std::string filename);

private:
	void ObjectLoad(LevelData* levelData, nlohmann::json& object);

	int reservenum = 0;
};

