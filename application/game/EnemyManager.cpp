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

	LevelDataExchanger::SetObjectData(*enemyList.back(), data);

	enemyList.back()->Initialize();
	enemyList.back()->SetModel(ModelManager::GetModel(data.fileName));

	//イベント名を入れているなら、非アクティブ化してイベントが呼び出されたときに出現するようにする
	if (data.eventtrigerName != "")
	{
		enemyList.back()->eventName_ = data.eventtrigerName;
		enemyList.back()->mActive = false;
	}
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
		if (!enemy->mActive)continue;

		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemyList)
	{
		if (!enemy->mActive)continue;

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

std::list<Enemy*> EnemyManager::PopEventEnemy(std::string eventname_)
{
	std::list<Enemy*> enemys;

	for (auto& enemy : enemyList)
	{
		//呼び出しイベント名と持っているイベント名が一致する個体を呼び出す
		if (eventname_ == enemy->eventName_)
		{
			if (!enemy->mActive) {
				enemy->mActive = true;
				enemys.push_back(enemy.get());
			}
		}
	}

	return enemys;
}
