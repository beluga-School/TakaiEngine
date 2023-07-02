#include "EnemyManager.h"
#include "MathF.h"
#include "GEnemy.h"

void EnemyManager::Load(const LevelData::ObjectData& data)
{
	//とりあえずキューブで配置
	enemyList.emplace_back();
	enemyList.back() = std::make_unique<GEnemy>();

	//モデルとか設定する
	enemyList.back()->Initialize();

	//positionとかを設定
	LevelDataExchanger::SetObjectData(*enemyList.back(), data);
}

void EnemyManager::Initialize()
{
	for (auto &enemy : enemyList)
	{
		enemy->Initialize();
	}
}

void EnemyManager::Update()
{
	enemyList.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});
	for (auto& enemy : enemyList)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemyList)
	{
		enemy->Draw();
	}
}
