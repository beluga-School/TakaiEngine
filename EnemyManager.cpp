#include "EnemyManager.h"
#include "MathF.h"
#include "GEnemy.h"
#include "ClearDrawScreen.h"

void EnemyManager::Load(const LevelData::ObjectData& data)
{
	//�Ƃ肠�����L���[�u�Ŕz�u
	enemyList.emplace_back();
	enemyList.back() = std::make_unique<GEnemy>();

	//���f���Ƃ��ݒ肷��
	enemyList.back()->Initialize();

	//position�Ƃ���ݒ�
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
	}
}
