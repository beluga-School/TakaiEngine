#include "AttackTutorialUI.h"
#include "Texture.h"
#include "InstantDrawer.h"

void AttackTutorialUI::LoadResource()
{
	TextureManager::Load("Resources\\ui\\slimeIcon.png", "slimeIcon");
	TextureManager::Load("Resources\\ui\\enemyDownString.png", "enemyDownString");
}

void AttackTutorialUI::Update()
{
	BackUpdate();

	standbyTimer.Update();
	standbyTimer2.Update();
	attackTimer.Update();
	slimeAlphaT.Update();

	slimeSize.x = TEasing::OutQuad(1.0f, 1.2f, attackTimer.GetTimeRate());
	slimeSize.y = TEasing::OutBack(1.0f, 0.3f, attackTimer.GetTimeRate());
	slimePos.y = TEasing::OutQuad(YSTART, YEND, attackTimer.GetTimeRate());

	slimeColor.f4.w = TEasing::OutQuad(0.0f, 1.0f, slimeAlphaT.GetTimeRate());
	slimeColor.f4.w *= alphaTimer.GetTimeRate();

	stringColor.f4.w = 1.0f * alphaTimer.GetTimeRate();

	if (slimeAlphaT.GetEnd())
	{
		if (!attackTimer.GetStarted())attackTimer.Start();
	}
	if (attackTimer.GetEnd())
	{
		if (!standbyTimer.GetStarted())standbyTimer.Start();
	}
	if (standbyTimer.GetEnd())
	{
		if (!slimeAlphaT.GetReverseStarted())slimeAlphaT.ReverseStart();
	}
	if (slimeAlphaT.GetReverseEnd())
	{
		if(!standbyTimer2.GetStarted())standbyTimer2.Start();
	}
	if (standbyTimer2.GetEnd())
	{
		slimeAlphaT.Start();
		attackTimer.Reset();
		standbyTimer.Reset();
		standbyTimer2.Reset();
	}
}

void AttackTutorialUI::Draw()
{
	BackDraw();

	InstantDrawer::DrawRotaGraph(slimePos.x, slimePos.y + 5, slimeSize.x * 1.05f, slimeSize.y * 1.055f, 0, "slimeIcon", {0,0,0,slimeColor.f4.w * 0.5f}, InstantDrawer::Anchor::DOWN);
	InstantDrawer::DrawRotaGraph(slimePos.x, slimePos.y, slimeSize.x, slimeSize.y, 0, "slimeIcon", slimeColor, InstantDrawer::Anchor::DOWN);
	
	InstantDrawer::DrawRotaGraph(stringPos.x, stringPos.y, 1.0f,1.0f, 0, "enemyDownString", stringColor);
}

void AttackTutorialUI::Move(UIMove uimove)
{
	switch (uimove)
	{
	case UIMove::START:
		alphaTimer.NoInitStart();
		slimeAlphaT.Start();
		attackTimer.Reset();
		break;
	case UIMove::END:
		alphaTimer.ReverseStart();
		break;
	}
}
