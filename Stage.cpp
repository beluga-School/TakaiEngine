#include "Stage.h"
#include "TimeManager.h"
#include "ImguiManager.h"
#include "MathF.h"

void Stage::SetStage1()
{
	SetBlock({ 0,-30,0 }, { 30,50,30 });
	SetBlock({ 30,-20,0 }, { 30,50,30 });
	SetBlock({ -30,-20,0 }, { 30,50,30 });
	SetBlock({ 0,-20,30 }, { 30,50,30 });
	SetBlock({ 0,-20,-30 }, { 30,50,30 });
}

void Stage::SetBlock(Vector3 pos, Vector3 scale)
{
	blockList.emplace_back();
	blockList.back().position = pos;
	blockList.back().scale = scale;
	blockList.back().rotation = {0,0,0};
	//blockList.back().cubeCol.upPlane.normal = { 0,1,0 };
}

void Stage::Update()
{
	for (Block& block : blockList)
	{
		block.Update();
	}
}

void Stage::Draw()
{
	for (Block& block : blockList)
	{
		block.Draw();
	}
}

GUI gui3("hoge2");

void Stage::CheckMobCol(Mob& mob)
{
	Cube tempCol = mob.cubeCol;

	//仮移動
	//ここで移動値を足すのだから、移動値を先に決めておかなければならない
	tempCol.position += mob.moveValue;

	gui3.Begin({ 800,100 }, { 500,300 });

	for (Block& block : blockList)
	{
		//そのブロックの上面より上にいるか(そのブロックの上に立っているか)
		bool onG = tempCol.position.y > block.position.y + block.scale.y * 0.5f;

		ImGui::Text("onG %d", onG);

		//上面の当たり判定
		if (CubeCollision(tempCol, block.cubeCol))
		{
			if (onG)
			{
				while (CubeCollision(tempCol, block.cubeCol))
				{
					tempCol.position.y += 0.1f;
					mob.jumpPower = 0;
					mob.moveValue.y += 0.1f;
					mob.onGround = true;
				}
			}
			//右面の当たり判定
			if (mob.moveValue.x > 0 && onG == false)
			{
				while (mob.moveValue.x >= 0)
				{
					tempCol.position.x -= 0.1f;
					mob.moveValue.x -= 0.1f;
				}
				mob.moveValue.x = 0;
				ImGui::Text("migi");
			}
			//左面の当たり判定
			if (mob.moveValue.x < 0 && onG == false)
			{
				while (mob.moveValue.x <= 0)
				{
					tempCol.position.x += 0.1f;
					mob.moveValue.x += 0.1f;
				}
				mob.moveValue.x = 0;
				ImGui::Text("hidari");
			}
			//正面の当たり判定
			if (mob.moveValue.z > 0 && onG == false)
			{
				while (mob.moveValue.z >= 0)
				{
					tempCol.position.z -= 0.1f;
					mob.moveValue.z -= 0.1f;
				}
				mob.moveValue.z = 0;
				ImGui::Text("mae");
			}
			//後面の当たり判定
			if (mob.moveValue.z < 0 && onG == false)
			{
				while (mob.moveValue.z <= 0)
				{
					tempCol.position.z += 0.1f;
					mob.moveValue.z += 0.1f;
				}
				mob.moveValue.z = 0;
				ImGui::Text("usiro");
			}
		}
	}

	gui3.End();
}
