#pragma once
#include "EventBlock.h"
#include "TEasing.h"
#include "Enemy.h"

class EnemyDokan : public EventBlock
{
public:
	EnemyDokan() :EventBlock()
	{
		SetTag(TagTable::EnemyDokan);
	};

	static void LoadResource();

	void Initialize()override;

	void Update()override;

	void Draw()override;

	void HitEffect()override;

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

