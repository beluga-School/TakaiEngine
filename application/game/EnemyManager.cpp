#include "EnemyManager.h"
#include "MathF.h"
#include "GEnemy.h"
#include "ClearDrawScreen.h"
#include "Player.h"
#include "CollideManager.h"
#include "Stage.h"

void EnemyManager::Load(const LevelData::ObjectData& data)
{
	enemyList.emplace_back();
	enemyList.back() = std::make_unique<GEnemy>();
	//繝｢繝・Ν縺ｨ縺玖ｨｭ螳壹☆繧・
	enemyList.back()->Initialize();
	enemyList.back()->SetModel(ModelManager::GetModel(data.fileName));

	//position縺ｨ縺九ｒ險ｭ螳・
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
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
		enemy->DrawOutLine();

		BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
		enemy->Draw();

		if (mIsDrawEncountSphere)
		{
			enemy->EncountSphereDraw("Toon");
		}

		if (StageChanger::Get()->mShowCollider)
		{
			enemy->box.DrawMaterial();
		}
	}
}
