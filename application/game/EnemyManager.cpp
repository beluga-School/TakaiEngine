#include "EnemyManager.h"
#include "MathF.h"
#include "Slime.h"
#include "ClearDrawScreen.h"
#include "Player.h"
#include "CollideManager.h"
#include "StageChanger.h"

void EnemyManager::Load(const LevelData::ObjectData& data)
{
	enemyList.emplace_back();
	enemyList.back() = std::make_unique<Slime>();
	enemyList.back()->Initialize();
	enemyList.back()->SetModel(ModelManager::GetModel(data.fileName));

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
		if (enemy->CheckTag(TagTable::DitherTransparent))
		{
			BasicObjectPreDraw("DitherOutline", false);
		}
		else
		{
			BasicObjectPreDraw("OutLine", false);
		}
		enemy->DrawOutLine();
		if (enemy->CheckTag(TagTable::DitherTransparent))
		{
			BasicObjectPreDraw("CharactorDither");
		}
		else
		{
			BasicObjectPreDraw("Toon");
		}
		enemy->Draw();

		if (mIsDrawEncountSphere)
		{
			enemy->EncountSphereDraw("Toon");
		}

		if (StageChanger::Get()->mShowCollider)
		{
			enemy->box.Draw();
		}
	}
}
