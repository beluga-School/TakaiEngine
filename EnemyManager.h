#pragma once
#include "LevelLoader.h"
#include "Enemy.h"
#include "ImguiManager.h"

//全ての敵を管理するEnemyManagerがいて
//そいつが敵のローダーとしての機能ももつ

class EnemyManager
{
public:
	//いったんeventtrigerに"enemy"って書いてあったらGEnemyを配置する設計に
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
private:
	GUI gui3 = {"check"};
};

