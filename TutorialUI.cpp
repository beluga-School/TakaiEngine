#include "TutorialUI.h"
#include "InstantDrawer.h"

void TutorialUI::BackUpdate()
{
	alphaTimer.Update();
	backColor.f4.w = TEasing::OutQuad(0, 0.25f, alphaTimer.GetTimeRate());
}

void TutorialUI::BackDraw()
{
	InstantDrawer::DrawBox(backpos.x, backpos.y, Util::WIN_WIDTH, Util::WIN_HEIGHT / 3, backColor);
}
