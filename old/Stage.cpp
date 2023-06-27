#include "Stage.h"
#include "TimeManager.h"
#include "ImguiManager.h"
#include "MathF.h"

void Stage::SetStage1()
{
	//ÉXÉ^Å[Égínì_âÒÇË
	SetBlock({  0,-80,0 }, { 30,100,30 });
	SetBlock({  30,-80,0 }, { 30,100,30 });
	SetBlock({  -30,-80,0 }, { 30,100,30 });
	
	SetBlock({  0,-40,-30 }, { 30,30,30 });
	SetBlock({  30,-40,-30 }, { 30,30,30 });
	SetBlock({  -30,-40,-30 }, { 30,30,30 });

	
	SetBlock({  0,-30,-70 }, { 30,30,30 });
	SetBlock({  30,-30,-70 }, { 30,30,30 });
	SetBlock({  -30,-30,-70 }, { 30,30,30 });

	SetBlock({  0,-20,-150 }, { 28,30,30 });
	SetBlock({  28,-20,-150 }, { 28,30,30 });
	SetBlock({  -28,-20,-150 }, { 28,30,30 });

	SetBlock({  0,0,-200 }, { 30,30,100 });
	SetBlock({ 30,0,-200 }, { 30,30,100 });
	SetBlock({ -30,0,-200 }, { 30,30,100 });

	//1ëwñ⁄íÜî’
	SetBlock({ -50,40,-300 }, { 30,100,30 });

	SetBlock({ 0,90,-380 }, { 30,30,30 });
	SetBlock({ 30,90,-380 }, { 30,30,30 });
	SetBlock({ -30,90,-380 }, { 30,30,30 });

	SetBlock({ 50,80,-400 }, { 30,30,30 });
	SetBlock({ 80,70,-420 }, { 30,30,30 });
	SetBlock({ 120,60,-500 }, { 30,30,30 });
	SetBlock({ 70,90,-530 }, { 30,30,30 });


	SetBlock({ -50,100,-400 }, { 30,30,30 });
	SetBlock({ -40,110,-440 }, { 30,30,30 });
	SetBlock({ -30,120,-480 }, { 30,30,30 });

	//ìoÇÈèÍèä
	SetBlock({ 0,180,-560 }, { 30,100,30 });

	//2äKëwñ⁄
	SetBlock({ 0,230,-480 }, { 100,10,100 });
	for (int32_t i = 0; i < 5; i++)
	{
		SetBlock({ 
			0,
			240 + i * 5.0f,
			-430 + 30.0f * i }, { 30,10,30 });
	}

	for (int32_t i = 0; i < 3; i++)
	{
		SetBlock({ 0,210,-280 +30.0f * i }, { 30,30,30 });
		SetBlock({ 30,210,-280 +30.0f * i }, { 30,30,30 });
		SetBlock({ -30,210,-280 +30.0f * i }, { 30,30,30 });
	}

	SetBlock({ 0,250,-100 }, { 30,30,100 });
	SetBlock({ 0,270,0 }, { 30,30,100 });
	SetBlock({ 0,290,100 }, { 30,30,100 });

}

void Stage::SetBlock(const Vector3& pos,const Vector3& scale)
{
	blockList.emplace_back();
	blockList.back().position = pos;
	blockList.back().scale = scale;
	blockList.back().rotation = {0,0,0};
	blockList.back().TEXTURE = TextureManager::GetTexture("blockTexture");
	blockList.back().color_ = { 0.8f ,0.8f ,0.8f ,1};
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