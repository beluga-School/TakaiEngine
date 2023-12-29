#pragma once
#include "Entity.h"
#include "TEasing.h"
#include "Enemy.h"

class EnemyDokan : public Entity
{
public:
	EnemyDokan() : Entity()
	{
		SetTag(TagTable::EnemyDokan);
		SetTag(TagTable::DitherTransparent);
	};

	//リソース読み込み
	static void LoadResource();

	void Initialize()override;

	void Update()override;

	void Draw()override;

	//エネミーを出現処理
	void PopEnemy();

public:
	//この土管の範囲内にこの数の敵がいるなら出現を止める
	int32_t maxEnemy = 3;

	int32_t countEnemy = 0;

	Sphere popEnemyCol = { {0,0,0} ,40.0f };

	//出現させたエネミーのポインタ
	Enemy* spawnedEnemy = nullptr;

private:
	TEasing::easeTimer popCool = 5.0f;
};

