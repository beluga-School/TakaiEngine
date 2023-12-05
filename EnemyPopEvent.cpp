#include "EnemyPopEvent.h"
#include "EventManager.h"
#include "EnemyManager.h"
#include "Slime.h"
#include "InstantDrawer.h"

void EnemyPopEvent::Initialize()
{
}

void EnemyPopEvent::Update()
{
	endTimer.Update();

	for (auto& enemy : enemys)
	{
		enemy->ForceState(ActTable::None);
	}
}

void EnemyPopEvent::Draw()
{
	InstantDrawer::DrawBox(Util::WIN_WIDTH / 2,200,500,100,Color(0.1f, 0.1f, 0.1f,0.9f));
	InstantDrawer::DrawGraph(Util::WIN_WIDTH / 2,200,1,1,"slaystring");
}

void EnemyPopEvent::Start()
{
	//事前に配置したイベント紐づけのエネミーを出現させる処理
	enemys = EnemyManager::Get()->PopEventEnemy("enemyPopEvent_Slime");

	endTimer.Start();
}

void EnemyPopEvent::End()
{
	EventManager::Get()->End("enemyPop");
	enemys.clear();
}

bool EnemyPopEvent::EndFlag()
{
	return endTimer.GetEnd();
}
