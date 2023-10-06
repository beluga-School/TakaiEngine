#pragma once
#include "LevelLoader.h"
#include "Enemy.h"
#include "ImguiManager.h"

//蜈ｨ縺ｦ縺ｮ謨ｵ繧堤ｮ｡逅・☆繧畿nemyManager縺後＞縺ｦ
//縺昴＞縺､縺梧雰縺ｮ繝ｭ繝ｼ繝繝ｼ縺ｨ縺励※縺ｮ讖溯・繧ゅｂ縺､

class EnemyManager
{
public:
	//縺・▲縺溘ｓeventtriger縺ｫ"enemy"縺｣縺ｦ譖ｸ縺・※縺ゅ▲縺溘ｉGEnemy繧帝・鄂ｮ縺吶ｋ險ｭ險医↓
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

	//繧ｨ繝ｳ繧ｫ繧ｦ繝ｳ繝医せ繝輔ぅ繧｢繧呈緒逕ｻ縺吶ｋ縺・
	bool mIsDrawEncountSphere = false;
private:

};

