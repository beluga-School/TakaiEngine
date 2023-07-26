#include "EnemyManager.h"
#include "MathF.h"
#include "GEnemy.h"
#include "ClearDrawScreen.h"
#include "Player.h"
#include "CollideManager.h"

void EnemyManager::Load(const LevelData::ObjectData& data)
{
	enemyList.emplace_back();
	enemyList.back() = std::make_unique<GEnemy>();
	//ƒ‚ƒfƒ‹‚Æ‚©Ý’è‚·‚é
	enemyList.back()->Initialize();

	//position‚Æ‚©‚ðÝ’è
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
	
		eCheck.Begin({ 300,100 }, { 200,300 });
		ImGui::Text("pos %f %f %f", enemy->position.x, enemy->position.y, enemy->position.z);
		ImGui::Text("colPos %f %f %f", enemy->box.cubecol.position.x, enemy->box.cubecol.position.y, enemy->box.cubecol.position.z);
		eCheck.End();
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
