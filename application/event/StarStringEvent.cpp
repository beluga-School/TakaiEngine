#include "StarStringEvent.h"
#include "InstantDrawer.h"
#include "GameUIManager.h"

void MessageEvent::Initialize()
{
	pos = { -1000,-1000 };
	startPos = { Util::WIN_WIDTH / 2, -200 };
	endPos = { Util::WIN_WIDTH / 2, 200 };
}

void MessageEvent::Update()
{
	spawnTimer.Update();
	invideTimer.Update();
	backTimer.Update();

	if (spawnTimer.GetEnd())
	{
		if (!invideTimer.GetStarted())invideTimer.Start();
	}
	if (invideTimer.GetEnd())
	{
		if (!backTimer.GetStarted())backTimer.Start();
	}
	if (backTimer.GetNowEnd())
	{
		GameUIManager::Get()->CDTimer.SetTime(20.0f);
		GameUIManager::Get()->CDTimer.Start();
	}

	if (spawnTimer.GetRun())
	{
		pos.x = TEasing::OutQuad(startPos.x, endPos.x, spawnTimer.GetTimeRate());
		pos.y = TEasing::OutQuad(startPos.y, endPos.y, spawnTimer.GetTimeRate());
	}
	if (backTimer.GetRun())
	{
		pos.x = TEasing::OutQuad(endPos.x, startPos.x, backTimer.GetTimeRate());
		pos.y = TEasing::OutQuad(endPos.y, startPos.y, backTimer.GetTimeRate());
	}
}

void MessageEvent::Draw()
{
	if (messageTexHandle != "") {
		InstantDrawer::DrawBox(pos.x, pos.y, 800, 100, Color(0.1f, 0.1f, 0.1f, 0.9f));
		InstantDrawer::DrawGraph(pos.x, pos.y, 1, 1, messageTexHandle);
	}
}

void MessageEvent::Start()
{
	spawnTimer.Start();
	invideTimer.Reset();
	backTimer.Reset();
}

void MessageEvent::End()
{
}

bool MessageEvent::EndFlag()
{
	return backTimer.GetEnd();
}
