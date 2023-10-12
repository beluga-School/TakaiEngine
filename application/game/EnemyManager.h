#pragma once
#include "LevelLoader.h"
#include "Enemy.h"
#include "ImguiManager.h"

class EnemyManager
{
public:
	void Load(const LevelData::ObjectData &data);

	static EnemyManager* Get()
	{
		static EnemyManager instance;
		return &instance;
	}

	void Initialize();
	void Update();
	void Draw();

	std::list<std::unique_ptr<Enemy>> enemyList;

	bool mIsDrawEncountSphere = false;
private:

};

