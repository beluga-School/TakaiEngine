#include "StarTutorialUI.h"
#include "InstantDrawer.h"

void StarTutorialUI::LoadResource()
{
	TextureManager::Load("Resources\\ui\\startutorial.png", "startutorial");
}

void StarTutorialUI::Update()
{
	alphaTimer.Update();
	stringColor.f4.w = TEasing::OutQuad(0, 1.0f, alphaTimer.GetTimeRate());
	backColor.f4.w = TEasing::OutQuad(0, 0.25f, alphaTimer.GetTimeRate());

	starpos.y = stringpos.y - 100;

	//星の出現処理
	sBackAlphaT.Update();
	starBackC.f4.w = TEasing::OutQuad(0, 1.0f, sBackAlphaT.GetTimeRate());
	if (sBackAlphaT.GetNowEnd())
	{
		sSpawnT.Start();
	}

	sSpawnT.Update();
	starColor.f4.w = TEasing::OutQuad(0, 1.0f, sSpawnT.GetTimeRate());

	if (sSpawnT.GetNowEnd())
	{
		sAngleTimer.Start();
	}

	//星の回転処理
	sAngleTimer.Update();
	sStandbyTimer.Update();
	
	starAngle = TEasing::OutBack(0, 360, sAngleTimer.GetTimeRate());
	if (sAngleTimer.GetNowEnd())
	{
		sStandbyTimer.Start();
	}
	if (sStandbyTimer.GetNowEnd())
	{
		sBackAlphaT.Reset();
		sSpawnT.ReverseStart();
	}
	sStandbyTimer2.Update();
	if (sSpawnT.GetReverseEnd())
	{
		sStandbyTimer2.NoInitStart();
	}
	if (sStandbyTimer2.GetNowEnd())
	{
		sBackAlphaT.Start();
		sStandbyTimer2.Reset();
		sSpawnT.Reset();
		sStandbyTimer.Reset();
		sAngleTimer.Reset();
	}
}

void StarTutorialUI::Draw()
{
	InstantDrawer::DrawBox(backpos.x, backpos.y, Util::WIN_WIDTH, Util::WIN_HEIGHT / 3,backColor);
	InstantDrawer::DrawRotaGraph(stringpos.x,stringpos.y, 1.0f,1.0f,0,"startutorial", stringColor);
	
	Color tempC = {0.1f,0.1f,0.1f,starBackC.f4.w};
	tempC.f4.w *= alphaTimer.GetTimeRate();
	InstantDrawer::DrawRotaGraph(starpos.x, starpos.y, 0.8f, 0.8f, starAngle,"starshadow", tempC);
	tempC = starBackC;
	tempC.f4.w *= alphaTimer.GetTimeRate();
	InstantDrawer::DrawRotaGraph(starpos.x, starpos.y, 0.8f, 0.8f, starAngle,"starblank", tempC);
	tempC = starColor;
	tempC.f4.w *= alphaTimer.GetTimeRate();
	InstantDrawer::DrawRotaGraph(starpos.x, starpos.y, 0.8f, 0.8f,starAngle,"star2d", tempC);
}

void StarTutorialUI::Appearance()
{
	alphaTimer.NoInitStart();
	sBackAlphaT.NoInitStart();
}

void StarTutorialUI::Leave()
{
	alphaTimer.ReverseStart();
}

void StarTutorialUI::Move(UIMove uimove)
{
	switch (uimove)
	{
	case UIMove::START:
		Appearance();
		break;
	case UIMove::END:
		Leave();
		break;
	}
}

void StarTutorialUI::Gui()
{
	tutorialUIgui.Begin({200,200},{300,300});
	ImGui::SliderFloat("stringX", &stringpos.x, -100, Util::WIN_WIDTH + 100);
	ImGui::SliderFloat("stringY", &stringpos.y, -100, Util::WIN_HEIGHT + 100);
	ImGui::SliderFloat("backX", &backpos.x, -100, Util::WIN_WIDTH + 100);
	ImGui::SliderFloat("backY", &backpos.y, -100, Util::WIN_HEIGHT + 100);
	ImGui::Text("starColor %f", &starColor.f4.w);
	tutorialUIgui.End();
}
