#include "BossArea.h"
#include "StageChanger.h"
#include "InstantDrawer.h"

void BossArea::Create()
{
	bossAreaParts.clear();

	//3つ目の壁を作ろうとしたときに、constbufferDataDisolve(というか多分定数バッファのどれに入ってもなるけど)
	//のunmapでエラーが起こる
	//マジで追っかけたくないので別のやり方を考えるか
	//わざわざ既存の当たり判定に頼らなくても、範囲さえ決めてしまえば座標は取れるわけで、
	//その中から出させないようにするって別に難しく無くね？
	//それでやろう

	//6方向に壁を制作
	////z-
	//bossAreaParts.emplace_back();
	//bossAreaParts.back().Initialize();
	//bossAreaParts.back().position = { position.x,position.y,position.z - (scale.z / 2) };
	//bossAreaParts.back().scale = { scale.x,scale.y,0.5f };
	////z+
	//bossAreaParts.emplace_back();
	//bossAreaParts.back().Initialize();
	//bossAreaParts.back().position = { position.x,position.y,position.z + (scale.z / 2) };
	//bossAreaParts.back().scale = { scale.x,scale.y,0.5f };
	////y-
	//bossAreaParts.emplace_back();
	//bossAreaParts.back().Initialize();
	//bossAreaParts.back().position = { position.x,position.y - (scale.y / 2),position.z };
	//bossAreaParts.back().scale = { scale.x,0.5f,scale.z };
	////y+
	//bossAreaParts.emplace_back();
	//bossAreaParts.back().position = { position.x,position.y + (scale.y / 2),position.z };
	//bossAreaParts.back().scale = { scale.x,0.5f,scale.z };
	////x-
	//bossAreaParts.emplace_back();
	//bossAreaParts.back().position = { position.x - (scale.x / 2),position.y,position.z };
	//bossAreaParts.back().scale = { 0.5f,scale.y,scale.z };
	////x+
	//bossAreaParts.emplace_back();
	//bossAreaParts.back().position = { position.x + (scale.x / 2),position.y,position.z };
	//bossAreaParts.back().scale = { 0.5f,scale.y,scale.z };
}

void BossArea::Initialize()
{
	Obj3d::Initialize();
	SetTexture(TextureManager::GetTexture("white"));
}

void BossArea::Update()
{
	for (auto& part : bossAreaParts)
	{
		part.Update();
	}
}

void BossArea::Draw()
{
	for (auto& part : bossAreaParts)
	{
		part.Draw();
	}
}