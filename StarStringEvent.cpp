#include "StarStringEvent.h"
#include "InstantDrawer.h"

void StarStringEvent::Initialize()
{
}

void StarStringEvent::Update()
{
	endTimer.Update();
}

void StarStringEvent::Draw()
{
	InstantDrawer::DrawBox(Util::WIN_WIDTH / 2, 200, 800, 100, Color(0.1f, 0.1f, 0.1f, 0.9f));
	InstantDrawer::DrawGraph(Util::WIN_WIDTH / 2, 200, 1, 1, "getstarstring");
}

void StarStringEvent::Start()
{
	endTimer.Start();
}

void StarStringEvent::End()
{
}

bool StarStringEvent::EndFlag()
{
	return endTimer.GetEnd();
}
