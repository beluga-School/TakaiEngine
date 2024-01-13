#include "DokanTutorialUI.h"
#include "Texture.h"
#include "InstantDrawer.h"

void DokanTutorialUI::LoadResource()
{
	TextureManager::Load("Resources\\ui\\dokanUp.png", "dokanUp");
	TextureManager::Load("Resources\\ui\\dokanDown.png", "dokanDown");
	TextureManager::Load("Resources\\ui\\dokanString.png", "dokanString");
	TextureManager::Load("Resources\\ui\\playerIcon.png", "playerIcon");
}

void DokanTutorialUI::Update()
{
	BackUpdate();

	timer.Update();
	standbyTimer.Update();
	standbyTimer2.Update();

	playerPos.y = TEasing::OutQuad(400, 450,timer.GetTimeRate());

	if (standbyTimer.GetRun())
	{
		playerColor.f4.w = TEasing::OutQuad(0.f, 1.f, standbyTimer.GetTimeRate());
		color.f4.w = TEasing::OutQuad(0.f, 1.f, standbyTimer.GetTimeRate());
	}
	if (standbyTimer2.GetRun())
	{
		playerColor.f4.w = 0;
		color.f4.w = TEasing::OutQuad(1.f, 0.f, standbyTimer2.GetTimeRate());
	}

	if (standbyTimer.GetNowEnd())
	{
		timer.Start();
	}
	if (timer.GetNowEnd())
	{
		standbyTimer2.Start();
	}
	if (standbyTimer2.GetNowEnd())
	{
		standbyTimer.Start();
		standbyTimer2.Reset();
		timer.Reset();
	}
}

void DokanTutorialUI::Draw()
{
	BackDraw();

	color.f4.w *= alphaTimer.GetTimeRate();
	playerColor.f4.w *= alphaTimer.GetTimeRate();

	Color stringColor = { 1,1,1,1 };
	stringColor.f4.w *= alphaTimer.GetTimeRate();

	InstantDrawer::DrawRotaGraph(dokanPos.x,dokanPos.y,1.0f,1.0f,0,"dokanUp",color);
	InstantDrawer::DrawRotaGraph(playerPos.x, playerPos.y,0.25f,0.25f,0,"playerIcon", playerColor);
	InstantDrawer::DrawRotaGraph(dokanPos.x,dokanPos.y,1.0f,1.0f,0,"dokanDown",color);
	InstantDrawer::DrawRotaGraph(stringPos.x, stringPos.y,1.0f,1.0f,0,"dokanString", stringColor);
}

void DokanTutorialUI::Move(UIMove uimove)
{
	switch (uimove)
	{
	case UIMove::START:
		alphaTimer.NoInitStart();
		Apparance();
		break;
	case UIMove::END:
		alphaTimer.ReverseStart();
		timer.ReverseStart();
		break;
	}
}

void DokanTutorialUI::Apparance()
{
	standbyTimer.Start();
	standbyTimer2.Reset();
	timer.Reset();
}
