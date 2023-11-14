#include "EnemyDokan.h"
#include "EnemyManager.h"
#include "GEnemy.h"

void EnemyDokan::LoadResource()
{
	ModelManager::Get()->LoadModel("red_dokan", "red_dokan", true);
}

void EnemyDokan::Initialize()
{
	SetModel(ModelManager::GetModel("red_dokan"));
	popEnemyCol.center = position;
}

void EnemyDokan::Update()
{
	popEnemyCol.center = position;

	Obj3d::Update(*Camera::sCamera);
}

void EnemyDokan::Draw()
{
	Obj3d::DrawMaterial();
}

void EnemyDokan::PopEnemy()
{
	//すでに湧いているなら飛ばす
	if (spawnedEnemy != nullptr)
	{
		//死んでるなら戻す
		if (spawnedEnemy->IsDead())
		{
			spawnedEnemy = nullptr;
		}
		return;
	}
	//空きがあるなら出現
	if (spawnedEnemy == nullptr)
	{
		spawnedEnemy = EnemyManager::Get()->PopEnemy<Slime>(position, { 0,0,0 }, { 1,1,1 });
	}
}
