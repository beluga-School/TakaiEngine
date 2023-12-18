#include "StarStringEvent.h"
#include "InstantDrawer.h"

void MessageEvent::Initialize()
{
}

void MessageEvent::Update()
{
	endTimer.Update();
}

void MessageEvent::Draw()
{
	if (messageTexHandle != "") {
		InstantDrawer::DrawBox(Util::WIN_WIDTH / 2, 200, 800, 100, Color(0.1f, 0.1f, 0.1f, 0.9f));
		InstantDrawer::DrawGraph(Util::WIN_WIDTH / 2, 200, 1, 1, messageTexHandle);
	}
}

void MessageEvent::Start()
{
	endTimer.Start();
}

void MessageEvent::End()
{
}

bool MessageEvent::EndFlag()
{
	return endTimer.GetEnd();
}
